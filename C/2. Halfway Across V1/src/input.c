#include "../include/halfway_across.h"

int inputIsNumeric(const char *input) {
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

char *takeInput() {
  char menuSelection[PROMPT_SIZE];
  while (fgets(menuSelection, sizeof(menuSelection), stdin)) {
    menuSelection[strcspn(menuSelection, "\n")] = '\0';
    if (inputIsNumeric(menuSelection)) {
      char *menuSelectionCopy = malloc(strlen(menuSelection) + 1);
      if (menuSelectionCopy) {
        snprintf(menuSelectionCopy, sizeof(menuSelectionCopy), "%s",
                 menuSelection);
        return menuSelectionCopy;
      } else {
        fprintf(stderr, "Error allocating memory for menu selection.\n");
        return NULL;
      }
    } else {
      return NULL;
    }
  }
  return NULL;
}

long convertInputToLong(char *menuSelection) {
  char *endptr;
  long selection = strtol(menuSelection, &endptr, 10);
  if (endptr == menuSelection || *endptr != '\0') {
    fprintf(stderr, "Error converting input to long\n");
    free(menuSelection);
    invalidInputMsg();
    return -1;
  }
  free(menuSelection);
  return selection;
}

int prompt(char *promptMessage) {
  printf("%s", promptMessage);
  char *menuSelection = takeInput();
  if (menuSelection != NULL) {
    long selection = convertInputToLong(menuSelection);
    return selection;
  } else {
    return -1;
  }
}