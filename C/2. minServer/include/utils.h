#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char *get_method(char *buffer);
int validate_path(char *file_request);
int log_request(char *buffer, int response_code, int response_size);

#endif