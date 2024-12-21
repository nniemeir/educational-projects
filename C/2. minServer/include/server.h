#ifndef SERVER_H
#define SERVER_H
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <unistd.h>

#define BACKLOG_SIZE 128
#define BUFFER_SIZE 16384
#define NULL_TERMINATOR_LENGTH 1

int init_server(int *port);

#endif