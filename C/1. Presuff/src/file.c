#include "../include/presuff.h"

char *constructFilePath(const char *directoryPath, const char *fileName) {
  char *filePath = malloc(strlen(directoryPath) + SLASH_LENGTH +
                          strlen(fileName) + NULL_TERMINATOR_LENGTH);
  if (filePath == NULL) {
    fprintf(stderr, "Error allocating memory for file path");
    return NULL;
  }
  snprintf(filePath,
           strlen(directoryPath) + SLASH_LENGTH + strlen(fileName) +
               NULL_TERMINATOR_LENGTH,
           "%s/%s", directoryPath, fileName);
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
  snprintf(baseName, baseNameLength + NULL_TERMINATOR_LENGTH, "%s", fileName);

  size_t directoryPathLength = strlen(directoryPath);
  size_t patternLength = strlen(pattern);
  size_t fileExtensionLength = strlen(fileExtension);

  char *newFilePath;
  if (mode == MODE_PREPEND) {
    size_t totalLength = directoryPathLength + SLASH_LENGTH + patternLength +
                         baseNameLength + fileExtensionLength +
                         NULL_TERMINATOR_LENGTH;
    newFilePath = malloc(totalLength);
    if (newFilePath == NULL) {
      fprintf(stderr, "Error allocating memory for basename");
      return NULL;
    }
    snprintf(newFilePath,
             strlen(directoryPath) + SLASH_LENGTH + strlen(pattern) +
                 baseNameLength + strlen(fileExtension) +
                 NULL_TERMINATOR_LENGTH,
             "%s/%s%s", directoryPath, pattern, fileName);
  } else if (mode == MODE_APPEND) {
    size_t totalLength = directoryPathLength + SLASH_LENGTH + patternLength +
                         baseNameLength + fileExtensionLength +
                         NULL_TERMINATOR_LENGTH;
    newFilePath = malloc(totalLength);
    if (newFilePath == NULL) {
      fprintf(stderr, "Error allocating memory for new file path");
      return NULL;
    }
    snprintf(newFilePath,
             strlen(directoryPath) + SLASH_LENGTH + strlen(pattern) +
                 baseNameLength + strlen(fileExtension) +
                 NULL_TERMINATOR_LENGTH,
             "%s/%s%s%s", directoryPath, baseName, pattern, fileExtension);
  } else {
    fprintf(stderr, "Invalid mode selection, this is likely a problem in the "
                    "modePrompt function");
    free(baseName);
    free(newFilePath);
    return NULL;
  }
  free(baseName);
  return newFilePath;
}

void renameFile(const char *oldFilePath, const char *newFilePath) {
  if (strlen(newFilePath) >= PATH_MAX) {
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
      snprintf(modifiedExtension, length, "%s", fileExtension);

      if (strcmp(modifiedExtension, filteredExtension) == 0 ||
          strcmp(filteredExtension, "None") == 0) {
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