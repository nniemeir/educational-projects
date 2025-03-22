// Author: Jacob Niemeir <nniemeir@protonmail.com>
// Prepend or append a string to each filename of the chosen extension in the
// chosen directory
#include "../include/presuff.h"

char *pathIsWorkDir(char *directoryPath, int pathSize)
{
  if (!getcwd(directoryPath, pathSize))
  {
    fprintf(stderr, "Failed to assign working directory to variable");
    return NULL;
  }
  directoryPath[pathSize - 1] = '\0';
  return directoryPath;
}

void process_args(int argc, char *argv[], char *dirArg, char *pattern, char *extArg, int *mode)
{
  opterr = 0;
  int modeArgsGiven = 0;
  int index, c;
  while ((c = getopt(argc, argv, "b::d:e::h::x:p:")) != -1)
  {
    switch (c)
    {
    case 'b':
      modeArgsGiven++;
      *mode = MODE_PREPEND;
      break;
    case 'd':
      if (!optarg)
      {
        fprintf(stderr, "Option -d requires an argument.\n");
        exit(EXIT_FAILURE);
      }
      snprintf(dirArg, PATH_MAX, "%s", optarg);
      break;
    case 'e':
      modeArgsGiven++;
      *mode = MODE_APPEND;
      break;
    case 'h':
      printHelpMsg();
      exit(EXIT_SUCCESS);
    case 'p':
      if (!optarg)
      {
        fprintf(stderr, "Option -p requires an argument.\n");
        exit(EXIT_FAILURE);
      }
      snprintf(pattern, NAME_MAX, "%s", optarg);
      break;
    case 'x':
      if (!optarg)
      {
        fprintf(stderr, "Option -x requires an argument.\n");
        exit(EXIT_FAILURE);
      }
      snprintf(extArg, NAME_MAX, "%s", optarg);
      break;
    case '?':
      fprintf(stderr, "Unknown option '-%c'. Run with -h for options.\n",
              optopt);
      exit(EXIT_FAILURE);
    }
  }
  for (index = optind; index < argc; index++)
    printf("Non-option argument %s\n", argv[index]);

  if (!mode)
  {
    printf("Missing mode argument.\n");
    exit(EXIT_FAILURE);
  }

  if (modeArgsGiven == 2)
  {
    printf("Conflicting mode arguments given.\n");
    exit(EXIT_FAILURE);
  }
}

int main(int argc, char *argv[])
{
  char directoryPath[PATH_MAX] = {0};
  char dirArg[PATH_MAX] = {0};
  char filteredExtension[NAME_MAX] = {0};
  char pattern[NAME_MAX] = {0};
  char extArg[NAME_MAX] = {0};
  int mode = 0;

  process_args(argc, argv, dirArg, pattern, extArg, &mode);

  if (dirArg[0] == '\0')
  {
    snprintf(directoryPath, sizeof(directoryPath), "%s",
             pathIsWorkDir(dirArg, sizeof(dirArg)));
  }
  else
  {
    snprintf(directoryPath, sizeof(directoryPath), "%s", dirArg);
  }

  if (extArg[0] == '\0')
  {
    snprintf(filteredExtension, sizeof(filteredExtension), "None");
  }
  else
  {
    snprintf(filteredExtension, sizeof(filteredExtension), "%s", extArg);
  }

  if (!patternRename(directoryPath, pattern, filteredExtension, mode))
  {
    fprintf(stderr, "Renaming files failed.\n");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}