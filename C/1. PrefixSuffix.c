// Author: Nat Niemeir
// This is a POSIX-compliant program to prepend or append a string to each filename of the chosen extension in the chosen directory 
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATH 4096
#define MAX_DIRECTORY_LENGTH 2048
#define MAX_PATTERN_LENGTH 256
#define MAX_EXTENSION_LENGTH 20

// Prepends or appends pattern to all files of specified extension in the specified directory
void patternRename(const char *directoryPath, const char *pattern,
             const char *filteredExtension, const char *mode) {
  DIR *dir;
  struct dirent *entry;

  // Open the directory
  dir = opendir(directoryPath);

  if (dir == NULL) {
    perror("Error opening directory");
    exit(EXIT_FAILURE);
  }

  printf("Prepending %s to %s files in %s/\n", pattern, filteredExtension, directoryPath);

  // The end of the directory has been reached once entry is NULL
  while ((entry = readdir(dir)) != NULL) {
    if (entry->d_type == DT_REG) { // Check if it is a regular file
      const char dot = '.';
      char *fileExtension = strrchr(entry->d_name, dot);
      if (fileExtension == NULL || fileExtension[0] == '\0') {
        // Handle empty string or null pointer
        return;
      }

      // Create a new string for the modified file extension
      size_t length = strlen(fileExtension);
      char modifiedExtension[length];
      strcpy(modifiedExtension, fileExtension + 1); // Skip the dot

      if (strcmp(modifiedExtension, filteredExtension) == 0) {
        // Allocate memory for filenames dynamically
        char *oldFilePath =
            malloc(strlen(directoryPath) + 1 + strlen(entry->d_name) + 1);
        char *newFilePath =
            malloc(strlen(directoryPath) + 1 + strlen(pattern) +
                   strlen(entry->d_name) + 1);

        // Construct file paths
        sprintf(oldFilePath, "%s/%s", directoryPath, entry->d_name);
	if (strcmp(mode, "1") == 0) {
        sprintf(newFilePath, "%s/%s%s", directoryPath, pattern,
                entry->d_name);
	}
	
	else {
	sprintf(newFilePath, "%s/%s%s", directoryPath, entry->d_name, pattern);
	}

	if(strlen(newFilePath) < MAX_PATH) {
	
        // Rename the file
        if (rename(oldFilePath, newFilePath) != 0) {
          printf("Error renaming file %s to %s\n", oldFilePath, newFilePath);
        } else {
          printf("File renamed: %s to %s\n", entry->d_name, newFilePath);
        }
        // Free dynamically allocated memory
        free(oldFilePath);
        free(newFilePath);
	}
	else {
	  printf("Maximum filename length exceeded for %s", oldFilePath);
	}
      }
    }
  }
  // Close the directory
  closedir(dir);
}

int main() {
  char directoryPath[MAX_DIRECTORY_LENGTH];
  char filteredExtension[MAX_EXTENSION_LENGTH];
  char pattern[MAX_PATTERN_LENGTH];
  char mode[2];
  system("clear");
  printf("Enter the directory's path: ");
  fgets(directoryPath, MAX_DIRECTORY_LENGTH, stdin);  
  directoryPath[strcspn(directoryPath, "\n")] = '\0'; 
  printf("Enter the file type to rename: ");
  fgets(filteredExtension, MAX_EXTENSION_LENGTH, stdin);
  filteredExtension[strcspn(filteredExtension, "\n")] = '\0';
  printf("Enter a pattern: ");
  fgets(pattern, MAX_PATTERN_LENGTH, stdin);
  pattern[strcspn(pattern, "\n")] = '\0';
  printf("1. Prepend\n2. Append\n ");
  fgets(mode, 2, stdin);
  mode[strcspn(mode, "\n")] = '\0';
  patternRename(directoryPath, pattern, filteredExtension, mode);
  return 0;
}
