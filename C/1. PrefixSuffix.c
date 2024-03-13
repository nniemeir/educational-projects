// Author: Nat Niemeir
// Prepend or append a string to each filename of the chosen extension in the chosen directory
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATH 4096
#define MAX_DIRECTORY_LENGTH 2048
#define MAX_PATTERN_LENGTH 256
#define MAX_EXTENSION_LENGTH 20

void patternRename(const char *directoryPath, const char *pattern,
                   const char *filteredExtension, const char *mode) {
  DIR *dir;
  struct dirent *entry;

  dir = opendir(directoryPath);

  if (dir == NULL) {
    fprintf(stderr, "Error opening directory:  %s", directoryPath);
    exit(EXIT_FAILURE);
  }
  
  while ((entry = readdir(dir)) != NULL) {
    if (entry->d_type == DT_REG) {
      const char dot = '.';
      char *fileExtension = strrchr(entry->d_name, dot);
      if (fileExtension == NULL || fileExtension[0] == '\0') {
        continue;
      }

      size_t length = strlen(fileExtension);
      char modifiedExtension[length];
      strcpy(modifiedExtension, fileExtension + 1); 
      
      if (strcmp(modifiedExtension, filteredExtension) == 0) {
        char *oldFilePath =
            malloc(strlen(directoryPath) + 1 + strlen(entry->d_name) + 1);
        char *newFilePath = malloc(strlen(directoryPath) + 1 + strlen(pattern) +
                                   strlen(entry->d_name) + 1);

        sprintf(oldFilePath, "%s/%s", directoryPath, entry->d_name);
        if (strcmp(mode, "1") == 0) {
          sprintf(newFilePath, "%s/%s%s", directoryPath, pattern,
                  entry->d_name);
        } else {
	  size_t baseNameLength = strlen(entry->d_name) - strlen(fileExtension);
	  char *baseName = malloc(baseNameLength + 1);
	  strncpy(baseName, entry->d_name, baseNameLength);
	  baseName[baseNameLength] = '\0';
          sprintf(newFilePath, "%s/%s%s%s", directoryPath, baseName,
                  pattern, fileExtension);
	  free(baseName);
        }

        if (strlen(newFilePath) < MAX_PATH) {
          if (rename(oldFilePath, newFilePath) != 0) {
            fprintf(stderr, "Error renaming file %s to %s\n", oldFilePath, newFilePath);
          }
          free(oldFilePath);
          free(newFilePath);

        } else {
          fprintf(stderr, "Maximum filename length exceeded for %s", oldFilePath);
        }
      }
    }
  }
  closedir(dir);
}

int main() {
  char directoryPath[MAX_DIRECTORY_LENGTH];
  char filteredExtension[MAX_EXTENSION_LENGTH];
  char pattern[MAX_PATTERN_LENGTH];
  char mode[2];
  printf("Enter the directory's absolute path: ");
  fgets(directoryPath, MAX_DIRECTORY_LENGTH, stdin);
  directoryPath[strcspn(directoryPath, "\n")] = '\0';
  printf("Enter the file type to rename (E.g. csv): ");
  fgets(filteredExtension, MAX_EXTENSION_LENGTH, stdin);
  filteredExtension[strcspn(filteredExtension, "\n")] = '\0';
  printf("Enter a pattern: ");
  fgets(pattern, MAX_PATTERN_LENGTH, stdin);
  pattern[strcspn(pattern, "\n")] = '\0';
  printf("1. Prepend\n2. Append\n ");
  fgets(mode, 2, stdin);
  mode[strcspn(mode, "\n")] = '\0';
  patternRename(directoryPath, pattern, filteredExtension, mode);
  return EXIT_SUCCESS;
}
