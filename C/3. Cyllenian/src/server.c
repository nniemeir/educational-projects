#include "../include/server.h"
#include "../include/response.h"

// Free the SSL object and context if they are currently allocated
void SSL_cleanup(SSL *ssl, SSL_CTX *ctx) {
  if (ssl) {
    SSL_free(ssl);
  }
  if (ctx) {
    SSL_CTX_free(ctx);
  }
}

// Sets up the SSL structure for client connection
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
  // Setup SSL connection with client
  SSL *ssl = setup_ssl(ctx, clientfd);
  if (!ssl) {
    return;
  }

  // Read the data sent by the client
  char buffer[BUFFER_SIZE];
  int bytes_read = SSL_read(ssl, buffer, sizeof(buffer) - 1);
  if (bytes_read <= 0) {
    fprintf(stderr, "Failed to read from connection.\n");
    SSL_cleanup(ssl, ctx);
    return;
  }
  buffer[bytes_read] = '\0';

  char *response = generate_response(buffer);
  if (response == NULL) {
    SSL_cleanup(ssl, ctx);
    return;
  }

  // After an appropriate response is generated, it is sent to the client
  if (SSL_write(ssl, response, strlen(response)) <= 0) {
    fprintf(stderr, "Failed to write to connection.\n");
    free(response);
    SSL_cleanup(ssl, ctx);
    return;
  }

  free(response);

  // The connection is then closed
  if (SSL_shutdown(ssl) < 0) {
    fprintf(stderr, "Failed to shutdown connection.\n");
  }

  SSL_cleanup(ssl, ctx);
  close(clientfd);
}

// The TCP socket set up here is later used for our connections
int init_socket(int port) {
  // AF_INET specifies that we are using IPv4 protocols
  // SOCK_STREAM specifies that this is a standard TCP connection
  // SOCK_STREAM only has one valid protocol (IPPROTO_TCP)
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    fprintf(stderr, "Failed to create socket.\n");
    return -1;
  }

  //
  struct sockaddr_in socket_address;
  socket_address.sin_family = AF_INET;
  // The port must be in network byte order
  socket_address.sin_port = htons(port);
  // INADDR_ANY allows the socket to bind to any available interface
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

// Set up the SSL context
// The SSL context is a group of configuration settings for our connections
SSL_CTX *init_ssl_ctx(char **cert_path, char **key_path) {
  SSL_CTX *ctx = SSL_CTX_new(TLS_server_method());
  if (ctx == NULL) {
    fprintf(stderr, "Failed to create SSL context.\n");
    return NULL;
  }

  if (!SSL_CTX_use_certificate_chain_file(ctx, *cert_path)) {
    fprintf(stderr, "Failed to set certificate.\n");
    SSL_cleanup(NULL, ctx);
    return NULL;
  }

  if (!SSL_CTX_use_PrivateKey_file(ctx, *key_path, SSL_FILETYPE_PEM)) {
    fprintf(stderr, "Failed to set private key.\n");
    SSL_cleanup(NULL, ctx);
    return NULL;
  }
  return ctx;
}

// Accept incoming connections, creating a new process for each one
int client_loop(SSL_CTX *ctx, int sockfd) {
  int clientfd = accept(sockfd, NULL, NULL);
  if (clientfd == -1) {
    fprintf(stderr, "Failed to accept connection.\n");
    return 1;
  }

  // Processes created via forking do not share memory, making them preferable
  // to threads in this instance
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

// The main function for HTTPS server
int init_server(int *port, char **cert_path, char **key_path) {
  if (!OPENSSL_init_ssl(OPENSSL_INIT_LOAD_SSL_STRINGS |
                            OPENSSL_INIT_LOAD_CRYPTO_STRINGS,
                        NULL)) {
    printf("Failed to initialize OpenSSL globally.\n");
    return EXIT_FAILURE;
  }

  SSL_CTX *ctx = init_ssl_ctx(cert_path, key_path);

  if (!ctx) {
    return EXIT_FAILURE;
  }

  sockfd = init_socket(*port);

  if (sockfd == -1) {
    fprintf(stderr, "Failed to create socket.\n");
    return EXIT_FAILURE;
  }

  fprintf(stdout, "Listening on port %d...\n\n", *port);
  fflush(stdout);

  int serving_clients = 1;
  while (serving_clients) {
    serving_clients = client_loop(ctx, sockfd);
  }

  close(sockfd);
  return EXIT_SUCCESS;
}