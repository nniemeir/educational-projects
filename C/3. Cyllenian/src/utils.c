#include "../include/utils.h"
#include "../include/file.h"

// Determine if the HTTP method is supported by the server
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

char *extract_host(char *buffer) {
  char *host = strdup(buffer);
  if (!host) {
    return NULL;
  }
  char *substring = NULL;

  // Skip the request line
  char *first_line_end = strchr(host, '\n');
  if (!first_line_end) {
    free(host);
    return NULL;
  }
  substring = first_line_end + 1;

  // Skip to right after the Host label
  char *host_position = strstr(substring, "Host:");
  if (!host_position) {
    fprintf(stderr, "No host found.\n");
    free(host);
    return NULL;
  }
  host_position += 6;

  // Terminate the string after the hostname
  char *host_end = strchr(host_position, ':');
  if (host_end) {
    *host_end = '\0';
  }
  char *host_result = strdup(host_position);
  if (!host_result) {
    free(host);
    return NULL;
  }
  free(host);
  return host_result;
}

// Extract the header by truncating the response at the first carriage return
char *extract_header(char *buffer) {
  char *header_line = strdup(buffer);
  if (!header_line) {
    return NULL;
  }
  char *carriage_position = strchr(header_line, '\r');
  if (carriage_position) {
    *carriage_position = '\0';
  }
  return header_line;
}

// Gather relevant information about the request and print it to stdout
int log_request(char *buffer, int response_code, int response_size) {
  char *host = extract_host(buffer);
  if (!host) {
    fprintf(stderr, "Failed to extract host from request.\n");
  } else {
    printf("%s\n", host);
    free(host);
  }

  time_t t = time(NULL);
  struct tm tm = *localtime(&t);

  char *header = extract_header(buffer);
  if (!header) {
    fprintf(stderr, "Failed to extract header from request.");
    header = "Unknown header";
  }

  printf("[%d-%02d-%02d %02d:%02d:%02d] \"%s\" %d %d\n\n", tm.tm_year + 1900,
         tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, header,
         response_code, response_size);
  free(header);
  return 1;
}