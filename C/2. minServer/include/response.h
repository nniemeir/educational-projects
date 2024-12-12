#ifndef RESPONSE_H
#define RESPONSE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 16384
#define HTTP_200 "HTTP/1.0 200 OK\r\n\r\n"
#define HTTP_404 "HTTP/1.0 404 Not Found\r\n\r\n";
#define HTTP_403 "HTTP/1.0 403 Forbidden\r\n\r\n"
#define HTTP_405 "HTTP/1.0 405 Method Not Allowed\r\n\r\n"
#define HTTP_413 "HTTP/1.0 413 Content Too Large\r\n\r\n"
#define METHOD_LENGTH 5
#define NULL_TERMINATOR_LENGTH 1

char *generate_response(char *buffer);

#endif