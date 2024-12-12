#include "../include/file.h"

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
    return -1;
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