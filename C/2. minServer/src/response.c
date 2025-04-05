#include "../include/response.h"
#include "../include/file.h"
#include "../include/utils.h"

// Matches the response code to its corresponding HTTP header
// See Mozilla's HTTP documentation for further details about HTTP response codes
char *get_metadata(int response_code) {
  switch (response_code) {
  case 200:
    return HTTP_200;
    break;
  case 403:
    return HTTP_403;
    break;
  case 404:
    return HTTP_404;
    break;
  case 405:
    return HTTP_405;
    break;
  case 413:
    return HTTP_413;
    break;
  default:
    return NULL;
  }
}

// Reads the specified file into a variable
FILE *open_file(const char *file_request) {
  FILE *file = fopen(file_request, "r");
  if (file == NULL) {
    fprintf(stderr, "Unable to open file %s", file_request);
  }
  return file;
}

// Read file in chunks to avoid buffer overflow
size_t read_file_chunks(FILE *file, char *response, size_t file_size,
                        size_t metadata_length, size_t available_space) {
  size_t bytes_read = 0;
  while (bytes_read < file_size) {
    size_t chunk_size = file_size - bytes_read < available_space
                            ? file_size - bytes_read
                            : available_space;
    size_t read =
        fread(response + metadata_length + bytes_read, 1, chunk_size, file);
    if (read == 0) {
      if (ferror(file)) {
        fprintf(stderr, "Failed to read file.\n");
        fclose(file);
        break;
      }
      break;
    }
    bytes_read += read;
  }
  return bytes_read;
}

char *add_file_to_response(char *method, char *file_request, char *response,
                           int *response_code) {
  FILE *file = open_file(file_request);
  if (!file) {
    fclose(file);
    return NULL;
  }

  char *metadata = get_metadata(*response_code);
  if (!metadata) {
    fclose(file);
    fprintf(stderr, "Failed to match response code to string.\n");
    return NULL;
  }

  // Ensure that sending the requested file and appropriate metadata won't
  // overload the buffer
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

  if (strcmp(method, "GET") == 0) {
    read_file_chunks(file, response, file_size, metadata_length,
                     available_space);
  }

  fclose(file);
  return response;
}

// Allocates memory to response and ensures said memory is initialized to all
// zeros
char *allocate_response() {
  char *response = malloc(BUFFER_SIZE);
  if (!response) {
    return NULL;
  }
  memset(response, 0, BUFFER_SIZE);
  return response;
}

// The requested file path is extracted by skipping over the HTTP method and
// terminating at the next space
char *isolate_file_request(char *buffer_copy) {
  if (!buffer_copy) {
    fprintf(stderr, "Failed to make copy of buffer.\n");
    return NULL;
  }

  char *file_request = buffer_copy + METHOD_LENGTH;

  char *space_position = strchr(file_request, ' ');
  if (space_position) {
    *space_position = '\0';
  }

  return file_request;
}

// Generate an HTTP response based on the contents of the request buffer
char *generate_response(char *buffer) {
  char *response;
  response = allocate_response();
  if (!response) {
    fprintf(stderr, "Failed to allocate memory for response.\n");
    return NULL;
  }

  int response_code;
  char *method = get_method(buffer);
  if (!method) {
    response_code = 405;
    response = add_file_to_response(method, "website/405.html", response,
                                    &response_code);
    return response;
  }

  char *buffer_copy = strdup(buffer);

  char *file_request = isolate_file_request(buffer_copy);

  response_code = 200;
  int valid_path = validate_path(file_request);
  if (valid_path == -1) {
    file_request = "website/403.html";
    response_code = 403;
  }

  if (valid_path == 0 || (valid_path = 1 && !file_exists(file_request))) {
    file_request = "website/404.html";
    response_code = 404;
  }

  response =
      add_file_to_response(method, file_request, response, &response_code);

  free(buffer_copy);

  if (!response) {
    fprintf(stderr, "Failed to add file to response.\n");
    return NULL;
  }

  int response_size =
      (strlen(response) + NULL_TERMINATOR_LENGTH) * sizeof(response);

  if (!log_request(buffer, response_code, response_size)) {
    fprintf(stderr, "Failed to log request.");
    free(response);
    return NULL;
  }

  return response;
}
