// Author: Jacob Niemeir <nniemeir@protonmail.com>
// Prepend or append a string to each filename of the chosen extension in the
// chosen directory
#include "../include/presuff.h"

int main() {
  char directoryPath[MAX_DIRECTORY_LENGTH];
  char filteredExtension[MAX_EXTENSION_LENGTH];
  char pattern[MAX_PATTERN_LENGTH];
  long mode;
  strcpy(directoryPath,
         prompt("Enter the directory's absolute path: ", MAX_DIRECTORY_LENGTH));
  strcpy(filteredExtension, prompt("Enter the file type to rename (E.g. csv): ",
                                   MAX_EXTENSION_LENGTH));
  strcpy(pattern, prompt("Enter a pattern: ", MAX_PATTERN_LENGTH));
  mode = modePrompt("1. Prepend\n2. Append\n", MODE_LENGTH);
  if (patternRename(directoryPath, pattern, filteredExtension, mode)) {
    return EXIT_SUCCESS;
  } else {
    fprintf(stderr, "Renaming files failed.\n");
    return EXIT_FAILURE;
  }
}
