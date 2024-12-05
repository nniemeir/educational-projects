#include "../include/server.h"

void SSL_cleanup(SSL *ssl, SSL_CTX *ctx) {
  if (ssl) {
    SSL_free(ssl);
  }
  if (ctx) {
    SSL_CTX_free(ctx);
  }
}

void handle_client(SSL_CTX *ctx, int clientfd) {
  SSL *ssl = SSL_new(ctx);
  if (ssl == NULL) {
    fprintf(stderr, "Failed to create SSL structure.\n");
    SSL_cleanup(ssl, ctx);
    return;
  }

  if (!SSL_set_fd(ssl, clientfd)) {
    fprintf(stderr, "Failed to set file descriptor for SSL object.\n");
    SSL_cleanup(ssl, ctx);
    return;
  }

  if (SSL_accept(ssl) <= 0) {
    fprintf(stderr, "TLS/SSL handshake failed.\n");
    SSL_cleanup(ssl, ctx);
    return;
  }

  char buffer[BUFFER_SIZE];
  if (SSL_read(ssl, buffer, sizeof(buffer) - NULL_TERMINATOR_LENGTH) <= 0) {
    fprintf(stderr, "Failed to read from connection.\n");
    SSL_cleanup(ssl, ctx);
    return;
  }

  char *response = generate_response(buffer);
  if (response == NULL) {
    SSL_cleanup(ssl, ctx);
    return;
  }

  if (SSL_write(ssl, response, strlen(response)) <= 0) {
    fprintf(stderr, "Failed to write to connection.\n");
    free(response);
    SSL_cleanup(ssl, ctx);
    return;
  }

  free(response);

  if (SSL_shutdown(ssl) < 0) {
    fprintf(stderr, "Failed to shutdown connection.\n");
  }

  SSL_cleanup(ssl, ctx);
  close(clientfd);
}

int main(int argc, char *argv[]) {
  int port = 8080;
  // Handle command-line options
  int c;
  while ((c = getopt(argc, argv, "h::p:")) != -1) {
    switch (c) {
    case 'h':
      printf("Usage: simpleHTTPS [options]\n");
      printf("Options:\n");
      printf("  -h               Show this help message\n");
      printf("  -p               Specify port to listen on\n");
      return EXIT_SUCCESS;
    case 'p':
      if (optarg) {
        port = atoi(optarg);
      } else {
        fprintf(stderr, "No argument given for -p, defaulting to 8080.\n");
        break;
      }
      break;
    case '?':
      fprintf(stderr, "Unknown option '-%c'. Run with -h for options.\n",
              optopt);
      return EXIT_FAILURE;
    }
  }

  if (!OPENSSL_init_ssl(OPENSSL_INIT_LOAD_SSL_STRINGS |
                            OPENSSL_INIT_LOAD_CRYPTO_STRINGS,
                        NULL)) {
    printf("Failed to initialize OpenSSL globally.\n");
    return EXIT_FAILURE;
  }

  SSL_CTX *ctx = SSL_CTX_new(TLS_server_method());
  if (ctx == NULL) {
    fprintf(stderr, "Failed to create SSL context.\n");
    return EXIT_FAILURE;
  }

  if (!SSL_CTX_use_certificate_chain_file(ctx, "cert")) {
    fprintf(stderr, "Failed to set certificate.\n");
    SSL_cleanup(NULL, ctx);
    return EXIT_FAILURE;
  }

  if (!SSL_CTX_use_PrivateKey_file(ctx, "key", SSL_FILETYPE_PEM)) {
    fprintf(stderr, "Failed to set private key.\n");
    SSL_cleanup(NULL, ctx);
    return EXIT_FAILURE;
  }

  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    fprintf(stderr, "Failed to create socket.\n");
    return EXIT_FAILURE;
  }

  struct sockaddr_in socket_address;
  socket_address.sin_family = AF_INET;
  socket_address.sin_port = htons(port);
  socket_address.sin_addr.s_addr = INADDR_ANY;

  if (bind(sockfd, (struct sockaddr *)&socket_address, sizeof(socket_address)) == -1) {
    fprintf(stderr, "Failed to bind socket.\n");
    return EXIT_FAILURE;
  }

  if (listen(sockfd, BACKLOG_SIZE) == -1) {
    fprintf(stderr, "Failed to start listening.\n");
    return EXIT_FAILURE;
  }

  while (1) {
    int clientfd = accept(sockfd, NULL, NULL);
    if (clientfd == -1) {
      fprintf(stderr, "Failed to accept connection.\n");
      continue;
    }

    pid_t pid = fork();
    if (pid == -1) {
      fprintf(stderr, "Failed to fork process.\n");
      close(clientfd);
      continue;
    }

    if (pid == 0) {
      close(sockfd);
      handle_client(ctx, clientfd);
      return EXIT_SUCCESS;
    } else {
      close(clientfd);
    }
  }
  close(sockfd);
  return EXIT_SUCCESS;
}
