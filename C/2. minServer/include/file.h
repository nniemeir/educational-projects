#ifndef FILE_H
#define FILE_H
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int file_exists(char *filename);
int is_path_valid(const char *file_request);
void normalize_path(char *file_request);
size_t get_file_size(const char *file_request);

#endif