#include "../include/server.h"

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
  }
  return NULL;
}

// Serve the requested file and corresponding metadata
char *add_file_to_response(char *method, char *file_request, char *response,
                           int *response_code) {
  FILE *file = fopen(file_request, "r");
  if (file == NULL) {
    fprintf(stderr, "Unable to open file %s", file_request);
    return NULL;
  }

  char *metadata = get_metadata(*response_code);
  if (!metadata) {
    fprintf(stderr, "Failed to match error code to string.\n");
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
  if (!buffer_copy) {
    free(response);
    fprintf(stderr, "Failed to make copy of buffer.\n");
    return NULL;
  }

  char *file_request = buffer_copy + METHOD_LENGTH;
  char *space_position = strchr(file_request, ' ');
  if (space_position) {
    *space_position = '\0';
  }

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
  if (!response) {
    free(buffer_copy);
    fprintf(stderr, "Failed to add file to response.\n");
    return NULL;
  }

  int response_size =
      (strlen(response) + NULL_TERMINATOR_LENGTH) * sizeof(response);

  if (!log_request(buffer, response_code, response_size)) {
    fprintf(stderr, "Failed to log request.");
    free(buffer_copy);
    free(response);
    return NULL;
  }

  free(buffer_copy);

  return response;
}
