#include "../include/presuff.h"

void patternRename(const char *directoryPath, const char *pattern,
                   const char *filteredExtension, long mode) {
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
        if (mode == 1) {
          sprintf(newFilePath, "%s/%s%s", directoryPath, pattern,
                  entry->d_name);
        } else {
          size_t baseNameLength = strlen(entry->d_name) - strlen(fileExtension);
          char *baseName = malloc(baseNameLength + 1);
          strncpy(baseName, entry->d_name, baseNameLength);
          baseName[baseNameLength] = '\0';
          sprintf(newFilePath, "%s/%s%s%s", directoryPath, baseName, pattern,
                  fileExtension);
          free(baseName);
        }

        if (strlen(newFilePath) < MAX_PATH) {
          if (rename(oldFilePath, newFilePath) != 0) {
            fprintf(stderr, "Error renaming file %s to %s\n", oldFilePath,
                    newFilePath);
          }
          free(oldFilePath);
          free(newFilePath);

        } else {
          fprintf(stderr, "Maximum filename length exceeded for %s",
                  oldFilePath);
        }
      }
    }
  }
  closedir(dir);
}