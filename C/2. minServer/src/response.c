#include "../include/server.h"

int file_exists(char *filename) {
  struct stat buffer;
  return stat(filename, &buffer) == 0 ? 1 : 0;
}

// Restrict file access to website directory
int is_path_valid(const char *file_request) {
  // Reject file paths containing directory traversal patterns
  char *traversal_patterns[] = {
      "../",      "%2e%2e%2f", "%2e%2e/",         "..%2f",   "%2e%2e%5c",
      "%2e%2e\\", "..% 5c ",   "%252e%252e%255c", "..%255c", "..\\"};
  for (int i = 0;
       i < sizeof(traversal_patterns) / sizeof(traversal_patterns[0]); ++i) {
    if (strstr(file_request, traversal_patterns[i]) != NULL) {
      return 0;
    }
  }
  // Reject file paths that do not begin with website/
  if (strncmp(file_request, "website/", 8) != 0) {
    return 0;
  }
  return 1;
}

// Remove trailing and redundant slashes from request
void normalize_path(char *file_request) {
  char *src = file_request, *dst = file_request;
  while (*src) {
    // Skip leading slash and double slashes
    if (*src == '/' && (src == file_request || *(src + 1) == '/')) {
      src++;
    } else {
      *dst++ = *src++;
    }
  }
  // Remove trailing slash
  if (dst != file_request && *(dst - 1) == '/') {
    dst--;
  }
  *dst = '\0';
}

// Get the file size of the request in bytes
size_t get_file_size(const char *file_request) {
  FILE *file = fopen(file_request, "r");
  if (!file) {
    fprintf(stderr, "Failed to open file to determine size.\n");
    return (size_t)-1;
  }
  if (fseek(file, 0, SEEK_END) == -1) {
    fprintf(stderr, "fseek failed.\n");
    fclose(file);
    return (size_t)-1;
  }
  size_t size = ftell(file);
  if (size == (size_t)-1) {
    fprintf(stderr, "ftell failed.\n");
    fclose(file);
    return (size_t)-1;
  }
  fclose(file);
  return size;
}

// Serve the requested file and corresponding metadata
char *add_file_to_response(char *method, char *file_request, char *response,
                           int *response_code) {
  FILE *file = fopen(file_request, "r");
  if (file == NULL) {
    fprintf(stderr, "Unable to open file %s", file_request);
    return NULL;
  }
  char *metadata;
  // Set metadata based on response_code
  switch (*response_code) {
  case 200:
    metadata = HTTP_200;
    break;
  case 404:
    metadata = HTTP_404;
    break;
  case 405:
    metadata = HTTP_405;
    break;
  case 413:
    metadata = HTTP_413;
    break;
  }
  size_t file_size = get_file_size(file_request);
  size_t metadata_length = strlen(metadata);
  size_t available_space = BUFFER_SIZE - metadata_length;
  if (file_size > available_space) {
    fprintf(stderr, "File content exceeds buffer size!\n");
    fclose(file);
    *response_code = 413;
    return NULL;
  }

  memcpy(response, metadata, metadata_length);
  // See Mozilla's documentation for a list of HTTP request methods
  // https://developer.mozilla.org/en-US/docs/Web/HTTP/Methods
  if (strcmp(method, "GET") == 0) {
    size_t bytes_read = 0;
    while (bytes_read < file_size) {
      // Read file in chunks to avoid buffer overflow
      size_t chunk_size = file_size - bytes_read < available_space
                              ? file_size - bytes_read
                              : available_space;
      size_t read =
          fread(response + metadata_length + bytes_read, 1, chunk_size, file);
      if (read == 0) {
        if (ferror(file)) {
          fprintf(stderr, "Failed to read file.\n");
          fclose(file);
          return NULL;
        }
        break;
      }
      bytes_read += read;
    }
  }
  fclose(file);
  return response;
}

char *get_method(char *buffer) {
  if (strncmp(buffer, "GET", 3) == 0) {
    return "GET";
  }
  if (strncmp(buffer, "HEAD", 4) == 0) {
    return "HEAD";
  }
  return NULL;
}

int validate_path(char *file_request) {
  normalize_path(file_request);
  return is_path_valid(file_request);
}

int log_request(char *buffer, int response_code, int response_size) {
  char *host = strdup(buffer);
  char *substring = NULL;
  char *first_line_end = strchr(host, '\n');
  if (!first_line_end) {
    free(host);
    return -1;
  }
  substring = first_line_end + 1;
  char *host_position = strstr(substring, "Host:");
  if (!host_position) {
    fprintf(stderr, "No host found.\n");
    free(host);
    return -1;
  }
  host_position += 6;
  char *host_end = strchr(host_position, ':');
  if (host_end) {
    *host_end = '\0';
  }
  printf("%s\n", host_position);

  time_t t = time(NULL);
  struct tm tm = *localtime(&t);

  char *header_line = strdup(buffer);
  char *carriage_position = strchr(header_line, '\r');
  if (carriage_position) {
    *carriage_position = '\0';
  }

  printf("[%d-%02d-%02d %02d:%02d:%02d] \"%s\" %d %d\n\n", tm.tm_year + 1900,
         tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,
         header_line, response_code, response_size);
  free(header_line);
  return 1;
}

// Generate response to HTTP requests based on the method specified in the
// header
char *generate_response(char *buffer) {
  // Allocate memory for response string
  char *response = malloc(BUFFER_SIZE);
  int response_code;
  if (!response) {
    fprintf(stderr, "Failed to allocate memory for response.\n");
    return NULL;
  }
  memset(response, 0, BUFFER_SIZE);

  // Ensure method is supported by the server
  char *method = get_method(buffer);
  if (!method) {
    free(response);
    response_code = 405;
    response = add_file_to_response(method, "website/405.html", response,
                                    &response_code);
    return response;
  }

  // Isolate file_request
  char *buffer_copy = strdup(buffer);
  char *file_request = buffer_copy + METHOD_LENGTH;
  char *space_position = strchr(file_request, ' ');
  if (space_position) {
    *space_position = '\0';
  }

  int valid_path = validate_path(file_request);
  response_code = 200;

  if (!valid_path || !file_exists(file_request)) {
    file_request = "website/404.html";
    response_code = 404;
  }
  response =
      add_file_to_response(method, file_request, response, &response_code);
  int response_size =
      (strlen(response) + NULL_TERMINATOR_LENGTH) * sizeof(response);
  if (!log_request(buffer, response_code, response_size)) {
    fprintf(stderr, "Failed to log request.");
  }
  return response;
}
