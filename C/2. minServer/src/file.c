#include "../include/file.h"

int file_exists(char *filename) {
  struct stat buffer;
  return stat(filename, &buffer) == 0 ? 1 : 0;
}

// Restrict file access to website directory
int is_path_valid(const char *file_request) {
  char resolved_path[PATH_MAX];
  char *resolved_file = realpath(file_request, resolved_path);
  char *traversal_patterns[] = {
      "../",      "%2e%2e%2f", "%2e%2e/",         "..%2f",   "%2e%2e%5c",
      "%2e%2e\\", "..% 5c ",   "%252e%252e%255c", "..%255c", "..\\"};
  for (size_t i = 0;
       i < sizeof(traversal_patterns) / sizeof(traversal_patterns[0]); ++i) {
    if (strstr(resolved_file, traversal_patterns[i]) != NULL) {
      return 0;
    }
  }
  if (strncmp(resolved_file, "website/", 8) != 0) {
    return -1;
  }
  return 1;
}

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
  if (dst != file_request && *(dst - 1) == '/') {
    dst--;
  }
  *dst = '\0';
}

// Get the size of the requested file in bytes to know how many bytes to
// read into response string
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