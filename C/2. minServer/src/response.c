#include "../include/server.h"

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
    perror("Failed to open file to determine size.");
    return (size_t)-1;
  }
  if (fseek(file, 0, SEEK_END) == -1) {
    perror("fseek failed");
    fclose(file);
    return (size_t)-1;
  }
  size_t size = ftell(file);
  if (size == (size_t)-1) {
    perror("ftell failed.");
    fclose(file);
    return (size_t)-1;
  }
  fclose(file);
  return size;
}

// Serve the requested file and corresponding metadata
char *add_file_to_response(char *method, char *metadata, char *file_request,
                 char *response) {
  FILE *file = fopen(file_request, "r");
  if (file == NULL) {
    return NULL;
  }
  size_t file_size = get_file_size(file_request);
  size_t metadata_length = strlen(metadata);
  size_t available_space = BUFFER_SIZE - metadata_length;
  if (file_size > available_space) {
    fprintf(stderr, "File content exceeds buffer size!\n");
    fclose(file);
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
          perror("Failed to read file.");
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

char *get_metadata(int valid_path, int valid_method) {
  if (valid_path) {
    return HTTP_200;
  }
  return HTTP_404;
}

// Generate response to HTTP requests based on the method specified in the
// header
char *generate_response(char *buffer) {
  // Allocate memory for response string
  char *response = malloc(BUFFER_SIZE);
  if (!response) {
    perror("Failed to allocate memory.");
    return NULL;
  }
  memset(response, 0, BUFFER_SIZE);

  // Ensure method is supported by the server
  char *method = get_method(buffer);
  if (!method) {
    free(response);
    return add_file_to_response(method, "website/405.html", HTTP_405, response);
  }

  // Isolate file_request
  char *file_request = buffer + METHOD_LENGTH;
  char *space_position = strchr(file_request, ' ');
  if (space_position) {
    *space_position = '\0';
  }

  // Serve the response
  int valid_path = validate_path(file_request);
  char *metadata = get_metadata(valid_path, method != NULL);
  return add_file_to_response(method, metadata,
                    valid_path ? file_request : "website/404.html", response);
}
