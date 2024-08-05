// Author: Jacob Niemeir <nniemeir@protonmail.com>
// Prepend or append a string to each filename of the chosen extension in the
// chosen directory
#include "../include/presuff.h"

char *pathIsWorkDir(char *directoryPath, int pathSize) {
  if (getcwd(directoryPath, pathSize) != NULL) {
    directoryPath[pathSize - 1] = '\0';
    return directoryPath;
  } else {
    fprintf(stderr, "Failed to assign working directory to variable");
    return NULL;
  }
}

int main(int argc, char *argv[]) {
  char directoryPath[PATH_MAX];
  char dirArg[PATH_MAX];
  char filteredExtension[NAME_MAX];
  char pattern[NAME_MAX];
  char extArg[NAME_MAX];
  int mode;
  int modeArgsGiven;
  int index;
  int c;
  opterr = 0;
  mode = 0;
  modeArgsGiven = 0;
  while ((c = getopt(argc, argv, "b::d:e::h::x:p:")) != -1) {
    switch (c) {
    case 'b':
      modeArgsGiven++;
      mode = 1;
      break;
    case 'd':
      if (optarg) {
        snprintf(dirArg, sizeof(dirArg), "%s", optarg);
      } else {
        fprintf(stderr, "Option -d requires an argument.\n");
        return EXIT_FAILURE;
      }
      break;
    case 'e':
      modeArgsGiven++;
      mode = 2;
      break;
    case 'h':
      helpMsg();
      return EXIT_SUCCESS;
      break;
    case 'p':
      if (optarg) {
        snprintf(pattern, sizeof(pattern), "%s", optarg);
      } else {
        fprintf(stderr, "Option -p requires an argument.\n");
        return EXIT_FAILURE;
      }
      break;
    case 'x':
      if (optarg) {
        snprintf(extArg, sizeof(extArg), "%s", optarg);
      } else {
        fprintf(stderr, "Option -x requires an argument.\n");
        return EXIT_FAILURE;
      }
      break;
    case '?':
      if (optopt == 'd' || optopt == 'p' || optopt == 'x') {
        fprintf(stderr, "Option -%c requires an argument.\n", optopt);
      } else if (isprint(optopt)) {
        fprintf(stderr, "Unknown option `-%c'.\n", optopt);
      } else {
        fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
      }
      return EXIT_FAILURE;
    default:
      abort();
    }
  }

  for (index = optind; index < argc; index++)
    printf("Non-option argument %s\n", argv[index]);

  if (mode == 0) {
    printf("Missing mode argument.\n");
    exit(EXIT_FAILURE);
  }

  if (modeArgsGiven == 2) {
    printf("Conflicting mode arguments given.\n");
    exit(EXIT_FAILURE);
  }

  if (dirArg[0] == '\0') {
    snprintf(directoryPath, sizeof(directoryPath), "%s",
             pathIsWorkDir(dirArg, sizeof(dirArg)));
  } else {
    snprintf(directoryPath, sizeof(directoryPath), "%s", dirArg);
  }

  if (extArg[0] == '\0') {
    snprintf(filteredExtension, sizeof(filteredExtension), "None");
  } else {
    snprintf(filteredExtension, sizeof(filteredExtension), "%s", extArg);
  }

  if (patternRename(directoryPath, pattern, filteredExtension, mode)) {
    return EXIT_SUCCESS;
  } else {
    fprintf(stderr, "Renaming files failed.\n");
    return EXIT_FAILURE;
  }
  return 0;
}