#ifndef PRESUFF_H_
#define PRESUFF_H_

#include <ctype.h>
#include <dirent.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define MODE_PREPEND 1
#define MODE_APPEND 2
#define NULL_TERMINATOR_LENGTH 1
#define SLASH_LENGTH 1

int patternRename(const char *directoryPath, const char *pattern,
                  const char *filteredExtension, long mode);
void helpMsg();

#endif