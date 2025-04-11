#include "../include/halfway_across.h"

int input_is_numeric(const char *input) {
  if (*input == '\0') {
    return 0;
  }
  while (*input) {
    if (!isdigit(*input)) {
      return 0;
    }
    input++;
  }
  return 1;
}

static char *take_input(void) {
  char menu_selection[PROMPT_SIZE];
  while (fgets(menu_selection, sizeof(menu_selection), stdin)) {
    menu_selection[strcspn(menu_selection, "\n")] = '\0';
    if (!input_is_numeric(menu_selection)) {
      return NULL;
    }
    char *menu_selection_copy = malloc(strlen(menu_selection) + 1);
    if (!menu_selection_copy) {
      fprintf(stderr, "Error allocating memory for menu selection.\n");
      return NULL;
    }
    snprintf(menu_selection_copy, PROMPT_SIZE, "%s", menu_selection);
    return menu_selection_copy;
  }
  return NULL;
}

static long convert_input_to_long(char *menu_selection) {
  char *endptr;
  long selection = strtol(menu_selection, &endptr, 10);
  if (endptr == menu_selection || *endptr != '\0') {
    fprintf(stderr, "Error converting input to long\n");
    free(menu_selection);
    invalid_input_msg();
    return -1;
  }
  free(menu_selection);
  return selection;
}

int prompt(char *prompt_msg) {
  printf("%s", prompt_msg);
  char *menu_selection = take_input();
  if (!menu_selection) {
    return -1;
  }
  long selection = convert_input_to_long(menu_selection);
  return selection;
}