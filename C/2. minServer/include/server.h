#ifndef SERVER_H
#define SERVER_H
#include <arpa/inet.h>
#include <netinet/in.h>
#include <openssl/ssl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define BACKLOG_SIZE 128
#define BUFFER_SIZE 16384
#define HTTP_200 "HTTP/1.0 200 OK\r\n\r\n"
#define HTTP_404 "HTTP/1.0 404 Not Found\r\n\r\n";
#define HTTP_403 "HTTP/1.0 403 Forbidden\r\n\r\n"
#define HTTP_405 "HTTP/1.0 405 Method Not Allowed\r\n\r\n"
#define HTTP_413 "HTTP/1.0 413 Content Too Large\r\n\r\n"
#define METHOD_LENGTH 5
#define NULL_TERMINATOR_LENGTH 1

// File
int file_exists(char *filename);
int is_path_valid(const char *file_request);
void normalize_path(char *file_request);
size_t get_file_size(const char *file_request);

// Response
char *generate_response(char *buffer);

// Utils
char *get_method(char *buffer);
int validate_path(char *file_request);
int log_request(char *buffer, int response_code, int response_size);

#endif