#include "../include/presuff.h"

void clearScreen() { printf("\e[H\e[2J"); }

void clearStdin() {
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;
}
void invalidInputMessage() {
  clearScreen();
  printf("There was an error processing input.");
  clearStdin();
  clearScreen();
}


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
    char *menuSelectionCopy = malloc(strlen(menuSelection) + 1);
    if (menuSelectionCopy) {
      strcpy(menuSelectionCopy, menuSelection);
      return menuSelectionCopy;
    } else {
      fprintf(stderr, "Error allocating memory for menu selection.\n");
      return NULL;
    }
  }
  return NULL;
}

char *takeNumInput() {
  char menuSelection[PROMPT_SIZE];
  while (fgets(menuSelection, sizeof(menuSelection), stdin)) {
    menuSelection[strcspn(menuSelection, "\n")] = '\0';
    if (inputIsNumeric(menuSelection)) {
      char *menuSelectionCopy = malloc(strlen(menuSelection) + 1);
      if (menuSelectionCopy) {
        strcpy(menuSelectionCopy, menuSelection);
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
    invalidInputMessage();
    return -1;
  }
  free(menuSelection);
  return selection;
}

char *prompt(char *promptMessage, int maxInputLength) {
  printf("%s", promptMessage);
  char *menuSelection = takeInput(maxInputLength);
  return menuSelection;
}

long modePrompt(char *promptMessage, int maxInputLength) {
  printf("%s", promptMessage);
  char *menuSelection = takeNumInput(maxInputLength);
  if (menuSelection != NULL) {
    long selection = convertInputToLong(menuSelection);
    return selection;
  } else {
    return -1;
  }
}
