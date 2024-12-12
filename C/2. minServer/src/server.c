#include "../include/response.h"
#include "../include/server.h"

void SSL_cleanup(SSL *ssl, SSL_CTX *ctx) {
  if (ssl) {
    SSL_free(ssl);
  }
  if (ctx) {
    SSL_CTX_free(ctx);
  }
}

SSL *setup_ssl(SSL_CTX *ctx, int clientfd) {
  SSL *ssl = SSL_new(ctx);
  if (ssl == NULL) {
    fprintf(stderr, "Failed to create SSL structure.\n");
    SSL_cleanup(ssl, ctx);
    return NULL;
  }

  if (!SSL_set_fd(ssl, clientfd)) {
    fprintf(stderr, "Failed to set file descriptor for SSL object.\n");
    SSL_cleanup(ssl, ctx);
    return NULL;
  }

  if (SSL_accept(ssl) <= 0) {
    fprintf(stderr, "TLS/SSL handshake failed.\n");
    SSL_cleanup(ssl, ctx);
    return NULL;
  }
  return ssl;
}

void handle_client(SSL_CTX *ctx, int clientfd) {
  SSL *ssl = setup_ssl(ctx, clientfd);
  if (!ssl) {
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

int init_socket(int port) {
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    fprintf(stderr, "Failed to create socket.\n");
    return -1;
  }

  struct sockaddr_in socket_address;
  socket_address.sin_family = AF_INET;
  socket_address.sin_port = htons(port);
  socket_address.sin_addr.s_addr = INADDR_ANY;

  if (bind(sockfd, (struct sockaddr *)&socket_address,
           sizeof(socket_address)) == -1) {
    fprintf(stderr, "Failed to bind socket.\n");
    return -1;
  }

  if (listen(sockfd, BACKLOG_SIZE) == -1) {
    fprintf(stderr, "Failed to start listening.\n");
    return -1;
  }
  return sockfd;
}

SSL_CTX *init_ssl_ctx() {
  SSL_CTX *ctx = SSL_CTX_new(TLS_server_method());
  if (ctx == NULL) {
    fprintf(stderr, "Failed to create SSL context.\n");
    return NULL;
  }

  if (!SSL_CTX_use_certificate_chain_file(ctx, "cert")) {
    fprintf(stderr, "Failed to set certificate.\n");
    SSL_cleanup(NULL, ctx);
    return NULL;
  }

  if (!SSL_CTX_use_PrivateKey_file(ctx, "key", SSL_FILETYPE_PEM)) {
    fprintf(stderr, "Failed to set private key.\n");
    SSL_cleanup(NULL, ctx);
    return NULL;
  }
  return ctx;
}

int client_loop(SSL_CTX *ctx, int sockfd) {
  int clientfd = accept(sockfd, NULL, NULL);
  if (clientfd == -1) {
    fprintf(stderr, "Failed to accept connection.\n");
    return 1;
  }

  pid_t pid = fork();
  if (pid == -1) {
    fprintf(stderr, "Failed to fork process.\n");
    close(clientfd);
    return 1;
  }

  if (pid == 0) {
    close(sockfd);
    handle_client(ctx, clientfd);
    return 0;
  } else {
    close(clientfd);
  }
  return 1;
}

int main(int argc, char *argv[]) {
  // Default to port 8080 if port argument not given
  int port = 8080;
  process_args(argc, argv, &port);

  if (!OPENSSL_init_ssl(OPENSSL_INIT_LOAD_SSL_STRINGS |
                            OPENSSL_INIT_LOAD_CRYPTO_STRINGS,
                        NULL)) {
    printf("Failed to initialize OpenSSL globally.\n");
    return EXIT_FAILURE;
  }

  SSL_CTX *ctx = init_ssl_ctx();

  if (!ctx) {
    return EXIT_FAILURE;
  }

  int sockfd = init_socket(port);

  if (sockfd == -1) {
    fprintf(stderr, "Failed to create socket.\n");
    return EXIT_FAILURE;
  }

  fprintf(stdout, "Listening on port %d...\n\n", port);
  fflush(stdout);

  int serving_clients = 1;
  while (serving_clients) {
    serving_clients = client_loop(ctx, sockfd);
  }

  close(sockfd);
  return EXIT_SUCCESS;
}
