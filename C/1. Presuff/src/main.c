// Author: Jacob Niemeir <nniemeir@protonmail.com>
// Prepend or append a string to each filename of the chosen extension in the
// chosen directory
#include "../include/presuff.h"

char *path_is_working_dir(char *dir_path, int path_size) {
  if (!getcwd(dir_path, path_size)) {
    fprintf(stderr, "Failed to assign working directory to variable");
    return NULL;
  }
  dir_path[path_size - 1] = '\0';
  return dir_path;
}

void process_args(int argc, char *argv[], char *dir_arg, char *pattern,
                  char *ext_arg, int *mode) {
  opterr = 0;
  int mode_args_given = 0;
  int index, c;
  while ((c = getopt(argc, argv, "b::d:e::h::x:p:")) != -1) {
    switch (c) {
    case 'b':
      mode_args_given++;
      *mode = MODE_PREPEND;
      break;
    case 'd':
      if (!optarg) {
        fprintf(stderr, "Option -d requires an argument.\n");
        exit(EXIT_FAILURE);
      }
      snprintf(dir_arg, PATH_MAX, "%s", optarg);
      break;
    case 'e':
      mode_args_given++;
      *mode = MODE_APPEND;
      break;
    case 'h':
      print_help_msg();
      exit(EXIT_SUCCESS);
    case 'p':
      if (!optarg) {
        fprintf(stderr, "Option -p requires an argument.\n");
        exit(EXIT_FAILURE);
      }
      snprintf(pattern, NAME_MAX, "%s", optarg);
      break;
    case 'x':
      if (!optarg) {
        fprintf(stderr, "Option -x requires an argument.\n");
        exit(EXIT_FAILURE);
      }
      snprintf(ext_arg, NAME_MAX, "%s", optarg);
      break;
    case '?':
      fprintf(stderr, "Unknown option '-%c'. Run with -h for options.\n",
              optopt);
      exit(EXIT_FAILURE);
    }
  }
  for (index = optind; index < argc; index++)
    printf("Non-option argument %s\n", argv[index]);

  if (!mode) {
    printf("Missing mode argument.\n");
    exit(EXIT_FAILURE);
  }

  if (mode_args_given == 2) {
    printf("Conflicting mode arguments given.\n");
    exit(EXIT_FAILURE);
  }
}

int main(int argc, char *argv[]) {
  char dir_path[PATH_MAX] = {0};
  char dir_arg[PATH_MAX] = {0};
  char filtered_extension[NAME_MAX] = {0};
  char pattern[NAME_MAX] = {0};
  char ext_arg[NAME_MAX] = {0};
  int mode = 0;

  process_args(argc, argv, dir_arg, pattern, ext_arg, &mode);

  if (dir_arg[0] == '\0') {
    snprintf(dir_path, sizeof(dir_path), "%s",
             path_is_working_dir(dir_arg, sizeof(dir_arg)));
  } else {
    snprintf(dir_path, sizeof(dir_path), "%s", dir_arg);
  }

  if (ext_arg[0] == '\0') {
    snprintf(filtered_extension, sizeof(filtered_extension), "None");
  } else {
    snprintf(filtered_extension, sizeof(filtered_extension), "%s", ext_arg);
  }

  if (!pattern_rename(dir_path, pattern, filtered_extension, mode)) {
    fprintf(stderr, "Renaming files failed.\n");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}