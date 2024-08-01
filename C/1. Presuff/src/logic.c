#include "../include/presuff.h"

char *constructFilePath(const char *directoryPath, const char *fileName) {
  char *filePath = malloc(strlen(directoryPath) + NULL_TERMINATOR_LENGTH + strlen(fileName) + NULL_TERMINATOR_LENGTH);
  if (filePath == NULL) {
    fprintf(stderr, "Error allocating memory for file path");
    return NULL;
  }
  sprintf(filePath, "%s/%s", directoryPath, fileName);
  return filePath;
}

char *constructNewFilePath(const char *directoryPath, const char *pattern,
                           const char *fileName, long mode) {
  const char dot = '.';
  char *fileExtension = strrchr(fileName, dot);
  if (fileExtension == NULL || fileExtension[0] == '\0') {
    return NULL;
  }

  size_t baseNameLength = strlen(fileName) - strlen(fileExtension);
  char *baseName = malloc(baseNameLength + NULL_TERMINATOR_LENGTH);
  if (baseName == NULL) {
    fprintf(stderr, "Error allocating memory for basename");
    return NULL;
  }
  strncpy(baseName, fileName, baseNameLength);
  baseName[baseNameLength] = '\0';

  char *newFilePath;
  if (mode == MODE_PREPEND) {
    newFilePath = malloc(strlen(directoryPath) + NULL_TERMINATOR_LENGTH + strlen(pattern) +
                         strlen(fileName) + NULL_TERMINATOR_LENGTH);
    if (newFilePath == NULL) {
      fprintf(stderr, "Error allocating memory for basename");
    return NULL;
    }
    sprintf(newFilePath, "%s/%s%s", directoryPath, pattern, fileName);
  } else if (mode == MODE_APPEND) {
    newFilePath = malloc(strlen(directoryPath) + NULL_TERMINATOR_LENGTH + strlen(pattern) +
                         baseNameLength + strlen(fileExtension) + NULL_TERMINATOR_LENGTH);
    if (newFilePath == NULL) {
      fprintf(stderr, "Error allocating memory for new file path");
    return NULL;
    }
    sprintf(newFilePath, "%s/%s%s%s", directoryPath, baseName, pattern,
            fileExtension);
  } else {
    fprintf(stderr, "Invalid mode selection, this is likely a problem in the modePrompt function");
    free(baseName);
    free(newFilePath);
    return NULL;
  }
  free(baseName);
  return newFilePath;
}

void renameFile(const char *oldFilePath, const char *newFilePath) {
  if (strlen(newFilePath) >= MAX_PATH) {
    fprintf(stderr, "Maximum filename length exceeded for %s\n", oldFilePath);
    return;
  }

  if (rename(oldFilePath, newFilePath) != 0) {
    fprintf(stderr, "Error renaming file %s to %s\n", oldFilePath, newFilePath);
  }
}


int patternRename(const char *directoryPath, const char *pattern,
                   const char *filteredExtension, long mode) {
  DIR *dir;
  struct dirent *entry;

  dir = opendir(directoryPath);
  if (dir == NULL) {
    fprintf(stderr, "Error opening directory: %s\n", directoryPath);
  return 0;
  }

  while ((entry = readdir(dir)) != NULL) {
    if (entry->d_type == DT_REG) {
      const char dot = '.';
      char *fileExtension = strrchr(entry->d_name, dot);
      if (fileExtension == NULL || fileExtension[0] == '\0') {
        continue;
      }

      size_t length = strlen(fileExtension);
      char modifiedExtension[length + NULL_TERMINATOR_LENGTH];
      strcpy(modifiedExtension, fileExtension + NULL_TERMINATOR_LENGTH);

      if (strcmp(modifiedExtension, filteredExtension) == 0) {
        char *oldFilePath = constructFilePath(directoryPath, entry->d_name);
        char *newFilePath =
            constructNewFilePath(directoryPath, pattern, entry->d_name, mode);

        if (newFilePath != NULL) {
          renameFile(oldFilePath, newFilePath);
        }

        free(oldFilePath);
        free(newFilePath);
      }
    }
  }
  closedir(dir);
    return 1;
}