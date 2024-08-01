#ifndef PRESUFF_H_
#define PRESUFF_H_

#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATH 4096
#define MAX_DIRECTORY_LENGTH 2048
#define PROMPT_SIZE 255
#define MAX_PATTERN_LENGTH 256
#define MAX_EXTENSION_LENGTH 20
#define MODE_LENGTH 2
#define MODE_PREPEND 1
#define MODE_APPEND 2 
#define NULL_TERMINATOR_LENGTH 1

int patternRename(const char *directoryPath, const char *pattern,
                   const char *filteredExtension, long mode);
char *prompt(char *promptMessage, int maxInputLength);
long modePrompt(char *promptMessage, int maxInputLength);

#endif