#include "include/server.h"

void SSL_cleanup(SSL *ssl, SSL_CTX *ctx) {
  if (ssl) {
    SSL_free(ssl);
  }
  if (ctx) {
    SSL_CTX_free(ctx);
  }
}

int main() {
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

  int clientfd = accept(sockfd, NULL, NULL);
  if (clientfd == -1) {
    perror("Failed to start accept");
    return EXIT_FAILURE;
  }

  SSL_CTX *ctx = SSL_CTX_new(TLS_server_method());
  if (ctx == NULL) {
    perror("Failed to create SSL context");
    return EXIT_FAILURE;
  }

  SSL *ssl = SSL_new(ctx);
  if (ssl == NULL) {
    perror("Failed to create SSL structure");
    SSL_cleanup(ssl, ctx);
    return EXIT_FAILURE;
  }

  if (!SSL_set_fd(ssl, clientfd)) {
    perror("Failed to set file descriptor for SSL object");
    SSL_cleanup(ssl, ctx);
    return EXIT_FAILURE;
  }

  if (!SSL_use_certificate_chain_file(ssl, "cert")) {
    perror("Failed to set certificate");
    SSL_cleanup(ssl, ctx);
    return EXIT_FAILURE;
  }

  if (!SSL_use_PrivateKey_file(ssl, "key", SSL_FILETYPE_PEM)) {
    perror("Failed to set private key");
    SSL_cleanup(ssl, ctx);
    return EXIT_FAILURE;
  }

  if (SSL_accept(ssl) <= 0) {
    perror("TLS/SSL handshake failed");
    SSL_cleanup(ssl, ctx);
    return EXIT_FAILURE;
  }

  char buffer[BUFFER_SIZE];
  if (SSL_read(ssl, buffer, sizeof(buffer) - NULL_TERMINATOR_LENGTH) <= 0) {
    perror("Failed to read from connection.");
    SSL_cleanup(ssl, ctx);
    return EXIT_FAILURE;
  }

  char *response = create_response(buffer);
  if (response == NULL) {
    SSL_cleanup(ssl, ctx);
    return EXIT_FAILURE;
  }

  if (SSL_write(ssl, response, strlen(response)) <= 0) {
    perror("Failed to write to connection");
    free(response);
    SSL_cleanup(ssl, ctx);
    return EXIT_FAILURE;
  }

  free(response);

  if (SSL_shutdown(ssl) < 0) {
    perror("Failed to shutdown connection");
    return EXIT_FAILURE;
  }

  SSL_cleanup(ssl, ctx);
  close(clientfd);
  close(sockfd);

  return EXIT_SUCCESS;
}
