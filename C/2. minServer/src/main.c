#include "../include/server.h"

void process_args(int argc, char *argv[], int *port) {
  int c;
  while ((c = getopt(argc, argv, "h::p:")) != -1) {
    switch (c) {
    case 'h':
      printf("Usage: simpleHTTPS [options]\n");
      printf("Options:\n");
      printf("  -h               Show this help message\n");
      printf("  -p               Specify port to listen on\n");
      exit(EXIT_SUCCESS);
    case 'p':
      if (optarg) {
        *port = atoi(optarg);
      } else {
        fprintf(stderr, "No argument given for -p, defaulting to 8080.\n");
        break;
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
  // Default to port 8080 if port argument not given
  int port = 8080;
  process_args(argc, argv, &port);
  return init_server(&port);
}