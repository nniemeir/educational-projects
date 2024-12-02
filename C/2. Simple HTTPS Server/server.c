#include "include/server.h"

void SSL_cleanup(SSL *ssl, SSL_CTX *ctx) {
  if (ssl) {
    SSL_free(ssl);
  }
  if (ctx) {
    SSL_CTX_free(ctx);
  }
}

void handle_client(int clientfd) {
  SSL_CTX *ctx = SSL_CTX_new(TLS_server_method());
  if (ctx == NULL) {
    perror("Failed to create SSL context.");
    return;
  }

  SSL *ssl = SSL_new(ctx);
  if (ssl == NULL) {
    perror("Failed to create SSL structure.");
    SSL_cleanup(ssl, ctx);
    return;
  }

  if (!SSL_set_fd(ssl, clientfd)) {
    perror("Failed to set file descriptor for SSL object.\n");
    SSL_cleanup(ssl, ctx);
    return;
  }

  if (!SSL_use_certificate_chain_file(ssl, "cert")) {
    perror("Failed to set certificate");
    SSL_cleanup(ssl, ctx);
    return;
  }

  if (!SSL_use_PrivateKey_file(ssl, "key", SSL_FILETYPE_PEM)) {
    perror("Failed to set private key");
    SSL_cleanup(ssl, ctx);
    return;
  }
  if (SSL_accept(ssl) <= 0) {
    perror("TLS/SSL handshake failed");
    SSL_cleanup(ssl, ctx);
    return;
  }

  char buffer[BUFFER_SIZE];
  if (SSL_read(ssl, buffer, sizeof(buffer) - NULL_TERMINATOR_LENGTH) <= 0) {
    perror("Failed to read from connection.");
    SSL_cleanup(ssl, ctx);
    return;
  }

  char *response = generate_response(buffer);
  if (response == NULL) {
    SSL_cleanup(ssl, ctx);
    return;
  }

  if (SSL_write(ssl, response, strlen(response)) <= 0) {
    perror("Failed to write to connection");
    free(response);
    SSL_cleanup(ssl, ctx);
    return;
  }

  free(response);

  if (SSL_shutdown(ssl) < 0) {
    perror("Failed to shutdown connection");
  }

  SSL_cleanup(ssl, ctx);
  close(clientfd);
}

int main() {
  if (!OPENSSL_init_ssl(OPENSSL_INIT_LOAD_SSL_STRINGS |
                            OPENSSL_INIT_LOAD_CRYPTO_STRINGS,
                        NULL)) {
    printf("Failed to initialize OpenSSL globally.");
    return EXIT_FAILURE;
  }
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    perror("Failed to create socket");
    return EXIT_FAILURE;
  }

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT);
  addr.sin_addr.s_addr = INADDR_ANY;

  if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
    perror("Failed to bind socket.");
    return EXIT_FAILURE;
  }

  if (listen(sockfd, BACKLOG_SIZE) == -1) {
    perror("Failed to start listening");
    return EXIT_FAILURE;
  }

  while (1) {
    int clientfd = accept(sockfd, NULL, NULL);
    if (clientfd == -1) {
      perror("Failed to accept connection.");
      continue;
    }

    pid_t pid = fork();
    if (pid == -1) {
      perror("Failed to fork process.");
      close(clientfd);
      continue;
    }

    if (pid == 0) {
      close(sockfd);
      handle_client(clientfd);
      return EXIT_SUCCESS;
    } else {
      close(clientfd);
    }
  }
  close(sockfd);
  return EXIT_SUCCESS;
}
