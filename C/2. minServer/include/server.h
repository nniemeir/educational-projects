#ifndef SERVER_H
#define SERVER_H
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <signal.h>
#include <unistd.h>

#define BACKLOG_SIZE 128
#define BUFFER_SIZE 16384
#define NULL_TERMINATOR_LENGTH 1

extern int sockfd;

int init_server(int *port, char **cert_path, char **key_path);

#endif