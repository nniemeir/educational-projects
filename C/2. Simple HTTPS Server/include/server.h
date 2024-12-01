#ifndef SERVER_H
#define SERVER_H
#include <arpa/inet.h>
#include <netinet/in.h>
#include <openssl/ssl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define BACKLOG_SIZE 128
#define BUFFER_SIZE 16384
#define HTTP_200 "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n"
#define HTTP_404 "HTTP/1.0 404 Not Found\r\n\r\n";
#define HTTP_405 "HTTP/1.0 405 Method Not Allowed\r\n\r\n"
#define METHOD_LENGTH 5
#define NULL_TERMINATOR_LENGTH 1
#define PORT 8080

char *create_response(char *buffer);

#endif