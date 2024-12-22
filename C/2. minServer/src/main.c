#include "../include/server.h"

char *construct_path_in_home(char *file_name) {
  const char *home = getenv("HOME");
  if (!home) {
    fprintf(stderr, "Failed to get value of HOME environment variable");
    return NULL;
  }
  const char *suffix = "/.local/share/minserver/";
  char *path = malloc(strlen(home) + strlen(suffix) + strlen(file_name) +
                      NULL_TERMINATOR_LENGTH);
  if (!path) {
    fprintf(stderr, "Failed to allocate memory for constructing file path.");
    return NULL;
  }
  strncpy(path, home, strlen(home));
  strcat(path, suffix);
  strcat(path, file_name);
  return path;
}

void process_args(int argc, char *argv[], int *port, char **cert_path,
                  char **key_path) {
  int c;
  while ((c = getopt(argc, argv, "c:h::k:p:")) != -1) {
    switch (c) {
    case 'c':
      *cert_path = optarg;
      break;
    case 'h':
      printf("Usage: minserver [options]\n");
      printf("Options:\n");
      printf("  -c               Specify path to certificate file\n");
      printf("  -h               Show this help message\n");
      printf("  -k               Specify path to private key file\n");
      printf("  -p               Specify port to listen on\n");
      exit(EXIT_SUCCESS);
    case 'k':
      *key_path = optarg;
      break;
    case 'p':
      *port = atoi(optarg);
      if (*port < 1024 || *port > 49151) {
        fprintf(
            stderr,
            "Invalid port specified, port must be between 1024 and 49151.\n");
        exit(EXIT_FAILURE);
      }
      break;
    case '?':
      fprintf(stderr, "Unknown option '-%c'. Run with -h for options.\n",
              optopt);
      exit(EXIT_FAILURE);
    }
  }
}

int main(int argc, char *argv[]) {
  // These values will be changed if their corresponding arguments are given
  int port = 8080;
  char *cert_path = construct_path_in_home("cert");
  char *key_path = construct_path_in_home("key");
  if (!cert_path || !key_path) {
    return EXIT_FAILURE;
  }

  process_args(argc, argv, &port, &cert_path, &key_path);
  int server_exit_status = init_server(&port, &cert_path, &key_path);
  free(cert_path);
  free(key_path);
  return server_exit_status;
}