#include "include/server.h"
#include <string.h>

int is_path_valid(const char *file_request) {
  char *traversal_patterns[] = {
      "../",      "%2e%2e%2f", "%2e%2e/",         "..%2f",   "%2e%2e%5c",
      "%2e%2e\\", "..% 5c ",   "%252e%252e%255c", "..%255c", "..\\"};
  int file_is_valid = 0;
  for (int i = 0;
       i < sizeof(traversal_patterns) / sizeof(traversal_patterns[0]); ++i) {
    if (strstr(file_request, traversal_patterns[i]) != NULL) {
      return 0;
    }
  }
  if (strncmp(file_request, "website/", sizeof("website/")) != 0) {
    return 0;
  }
  return 1;
}

void normalize_path(char *file_request) {
  char *src = file_request, *dst = file_request;
  int leading_slash = 1;
  while (*src) {
    if (*src == '/' && *(src + 1) == '/') {
      src++;
    } else {
      if (*src == '/') {
        leading_slash = 0;
      }
      *dst++ = *src++;
    }
  }
  *dst = '\0';
}

size_t get_file_size(const char *file_request) {
  FILE *page = fopen(file_request, "r");
  if (!page) {
    perror("Failed to open file to determine size.");
    return (size_t)-1;
  }
  if (fseek(page, 0, SEEK_END) == -1) {
    perror("fseek failed");
    fclose(page);
    return (size_t)-1;
  }
  size_t size = ftell(page);
  if (size == (size_t)-1) {
    perror("ftell failed.");
    fclose(page);
    return (size_t)-1;
  }
  fclose(page);
  return size;
}

char *serve_page(char *method, char *metadata, char *file_request,
                 char *response) {
  FILE *page = fopen(file_request, "r");
  if (page == NULL) {
    perror("Failed to open file.");
    return NULL;
  }

  size_t file_size = get_file_size(file_request);
  size_t metadata_length = strlen(metadata);
  size_t available_space = BUFFER_SIZE - metadata_length;
  if (file_size > available_space) {
    fprintf(stderr, "File content exceeds buffer size!\n");
    fclose(page);
    return NULL;
  }
  memcpy(response, metadata, metadata_length);

  if (strcmp(method, "GET") == 0) {
    fread(response + metadata_length, 1, file_size, page);
  }
  if (ferror(page)) {
    perror("Failed to read file.");
    fclose(page);
    return NULL;
  }
  fclose(page);
  return response;
}

char *create_response(char *buffer) {
  char *response = malloc(BUFFER_SIZE);
  if (!response) {
    perror("Failed to allocate memory.");
    return NULL;
  }
  memset(response, 0, BUFFER_SIZE);
  int valid_method = 0;
  char *method;
  if (strncmp(buffer, "GET", 3) == 0) {
    valid_method = 1;
    method = "GET";
  }
  if (strncmp(buffer, "HEAD", 4) == 0) {
    valid_method = 1;
    method = "HEAD";
  }
  char *file_request = buffer + METHOD_LENGTH;

  char *space_position = strchr(file_request, ' ');
  if (space_position) {
    *space_position = '\0';
  }

  normalize_path(file_request);
  char *metadata;
  char *page_not_found = "website/404.html";
  char *method_not_allowed_page = "website/405.html";
  if (valid_method) {
    int valid_path = is_path_valid(file_request);
    if (!valid_path) {
      file_request = page_not_found;
    }
    metadata = valid_path ? HTTP_200 : HTTP_404;
  } else {
    file_request = method_not_allowed_page;
    metadata = HTTP_405;
  }
  response = serve_page(method, metadata, file_request, response);

  if (!response) {
    free(response);
    return NULL;
  }
  return response;
}
