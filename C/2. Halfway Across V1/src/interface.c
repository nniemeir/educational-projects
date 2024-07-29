#include "../include/halfway_across.h"

#define INVENTORY_ARRAY_OFFSET 1

typedef enum { MAIN_NEW_JOURNAL = 1, MAIN_GO_HOME } MainMenuOption;

typedef enum { PREFACE_READ = 1, PREFACE_SKIP } PrefaceMenuOption;

typedef enum {
  CAMP_LEAVE = 1,
  CAMP_SHOW_INVENTORY,
  CAMP_REFLECT,
  CAMP_ADVANCE_DAY,
  CAMP_GIVE_UP
} CampMenuOption;

typedef enum {
  TRAVEL_LAKE = 1,
  TRAVEL_VALLEY,
  TRAVEL_NOWHERE
} TravelMenuOption;

typedef enum { VALLEY_REFLECT = 1, VALLEY_LEAVE } ValleyMenuOption;

typedef enum { LAKE_REFLECT = 1, LAKE_LEAVE } LakeMenuOption;

typedef enum { ITEM_USE = 1, ITEM_DISCARD, ITEM_EXIT } ItemMenuOption;

struct inventoryValues currentInventoryValues = {{"nothing"}, 0, 0, 0};

void clearScreen() { printf("\e[H\e[2J"); }

void clearStdin() {
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;
}

void invalidInputMessage() {
  clearScreen();
  printf("My memory is failing me at the moment...\n");
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

int prompt(char *promptMessage) {
  int selectionIncrement;
  printf(promptMessage);
  char *menuSelection = takeInput();
  if (menuSelection != NULL) {
    long selection = convertInputToLong(menuSelection);
    return selection;
  } else {
    return -1;
  }
}

void itemMenu(char *selectedItemName) {
  int selectedItemIndex = findItemIndex(selectedItemName);
  int validItemMenuSelection = 0;
  while (!validItemMenuSelection) {
    clearScreen();
    printf("%s\nQuantity: %d\n\n", selectedItemName,
           inventory.items[selectedItemIndex].amount,
           inventory.items[selectedItemIndex].equipped);
    int selection = prompt("1. Use / Equip\n2. Discard\n3. Exit\n\n> ");
    switch (selection) {
    case ITEM_USE:
      useItem(selectedItemIndex);
      clearStdin();
      validItemMenuSelection = 1;
      break;
    case ITEM_DISCARD:
      discardItem(selectedItemIndex);
      validItemMenuSelection = 1;
      break;
    case ITEM_EXIT:
      validItemMenuSelection = 1;
      clearScreen();
      printf("I chose to leave it be...\n");
      clearStdin();
      break;
    default:
      invalidInputMessage();
    }
  }
}

// If the player possesses at least one of a given item, that item's array
// number + 1 is added to the inventory prompt string along with its name.
char *generateInventoryPrompt(char *generatedInventoryPrompt) {
  strcpy(generatedInventoryPrompt,
         "I had a variety of items, I inspected my...\n\n");
  char buffer[100];
  for (int itemsIncrement = 0; itemsIncrement < NUM_OF_ITEMS;
       itemsIncrement++) {
    if (inventory.items[itemsIncrement].amount != 0) {
      strcpy(currentInventoryValues
                 .itemsPossessed[currentInventoryValues.itemCount],
             inventory.items[itemsIncrement].name);
      snprintf(buffer, sizeof(buffer), "%d",
               currentInventoryValues.itemCount + INVENTORY_ARRAY_OFFSET);
      strcat(generatedInventoryPrompt, buffer);
      strcat(generatedInventoryPrompt, ". ");
      strcat(generatedInventoryPrompt, inventory.items[itemsIncrement].name);
      strcat(generatedInventoryPrompt, "\n");
      currentInventoryValues.itemCount++;
    }
  }
  snprintf(buffer, sizeof(buffer), "%d",
           currentInventoryValues.itemCount + INVENTORY_ARRAY_OFFSET);
  strcat(generatedInventoryPrompt, buffer);
  strcat(generatedInventoryPrompt, ". Exit\n\n> ");
  return generatedInventoryPrompt;
}

// The value of each variable in the currentInventory struct is altered in
// inventoryMenu and its associated functions, so they must all be reset before
// running the functions again.
void resetInventoryValues() {
  for (int itemsPossessedIncrement = 0; itemsPossessedIncrement < NUM_OF_ITEMS;
       itemsPossessedIncrement++) {
    memset(currentInventoryValues.itemsPossessed[itemsPossessedIncrement], '\0',
           ITEM_NAME_SIZE);
  }
  currentInventoryValues.itemCount = 0;
  currentInventoryValues.numOfItemsPossessed = 0;
  currentInventoryValues.selection = 0;
}

void inventoryMenu() {
  int validInventorySelection = 0;
  while (!validInventorySelection) {
    resetInventoryValues();
    clearScreen();
    char generatedInventoryPrompt[PROMPT_SIZE];
    char *inventoryPrompt = generateInventoryPrompt(generatedInventoryPrompt);
    currentInventoryValues.selection = prompt(inventoryPrompt);
    int exitOption = currentInventoryValues.itemCount + 1;
    if (currentInventoryValues.selection > 0 &&
        currentInventoryValues.selection <= currentInventoryValues.itemCount) {
      // Since the prompt displays options starting from 1 instead of 0, this
      // offset must be corrected before matching the user's selection to the
      // corresponding name in the itemsPossessed array
      int fixedOffsetSelection =
          currentInventoryValues.selection - INVENTORY_ARRAY_OFFSET;
      itemMenu(currentInventoryValues.itemsPossessed[fixedOffsetSelection]);
    } else if (currentInventoryValues.selection == exitOption) {
      validInventorySelection = 1;
      clearScreen();
      printf("I switched my focus to another matter...\n");
      return;
    } else {
      invalidInputMessage();
    }
  }
}

void lakeMenu() {
  int leftLake = 0;
  while (!leftLake) {
    clearScreen();
    int selection = prompt("After reaching the lake, I decided to...\n\n1. "
                           "Reflect\n2. Return to Camp\n\n> ");
    switch (selection) {
    case LAKE_REFLECT:
      warnings();
      break;
    case LAKE_LEAVE:
      leftLake = 1;
      break;
    default:
      invalidInputMessage();
    }
  }
}

void valleyMenu() {
  int leftValley = 0;
  while (!leftValley) {
    clearScreen();
    int selection = prompt("After reaching the valley, I decided to...\n\n1. "
                           "Reflect\n2. Return to Camp\n\n> ");
    switch (selection) {
    case VALLEY_REFLECT:
      warnings();
      break;
    case VALLEY_LEAVE:
      leftValley = 1;
      break;
    default:
      invalidInputMessage();
    }
  }
}

void travelMenu() {
  int validTravelMenuSelection = 0;
  while (!validTravelMenuSelection) {
    clearScreen();
    int selection = prompt("I decided to walk to...\n\n1. The Lake\n2. The "
                           "Valley\n3. Nowhere\n\n> ");
    switch (selection) {
    case TRAVEL_LAKE:
      validTravelMenuSelection = 1;
      player.energy = 0;
      lakeMenu();
      break;
    case TRAVEL_VALLEY:
      validTravelMenuSelection = 1;
      player.energy = 0;
      valleyMenu();
      break;
    case TRAVEL_NOWHERE:
      validTravelMenuSelection = 1;
      break;
    default:
      invalidInputMessage();
    }
  }
}

void campMenu() {
  int gaveUp = 0;
  while (!gaveUp) {
    clearScreen();
    if (player.health <= 0) {
      clearScreen();
      printf("This is my last entry, my health is failing me. Whoever finds "
             "this journal is welcome to whatever is left in my camp...");
      clearStdin();
      clearScreen();
      return;
    }
    printf("San Juan Mountains, 1893\n");
    printf("Day %d\n\n", world.day);
    printf("It was %d degrees and %s today.\n", world.currentTemperature,
           world.currentWeather);
    int selection = prompt("I decided to...\n\n1. Leave Camp\n2. Examine My "
                           "Belongings\n3. Reflect\n4. Sleep\n5. "
                           "Give Up\n\n> ");
    switch (selection) {
    case CAMP_LEAVE:
      if (strcmp(world.currentWeather, "snowed heavily") == 0) {
        clearScreen();
        printf("It was snowing too heavily for me to travel...");
        clearStdin();
        clearScreen();
      } else if (player.energy == 0) {
        clearScreen();
        printf("I didn't have the energy to travel anywhere else today...");
        clearStdin();
        clearScreen();
      } else {
        travelMenu();
      }
      break;
    case CAMP_SHOW_INVENTORY:
      inventoryMenu();
      clearStdin();
      break;
    case CAMP_REFLECT:
      warnings();
      break;
    case CAMP_ADVANCE_DAY:
      advanceDay();
      break;
    case CAMP_GIVE_UP:
      gaveUp = 1;
      clearScreen();
      printf("The journal ends here...\n");
      clearStdin();
      break;
    default:
      invalidInputMessage();
    }
  }
}

void preface() {
  clearScreen();
  printf("Someday you'll find yourself compelled to disrupt a patch of dirt, "
         "thinking you'll dig up a better tomorrow.\n\n");
  sleep(2);
  printf("You'll dig until your hands are masses of blisters and you're as "
         "stiff as a board. Another hour and I'll certainly be there, you'll "
         "think to yourself.\n\n");
  sleep(2);
  printf("The sun might try to boil your skin, but you'll persist.\n\n");
  sleep(2);
  printf("The truth is, some holes don't want to be dug.\n\n");
  sleep(2);
  printf("Eventually there comes a time where you just have to put down the "
         "shovel and be on your way, or that hole will be your grave...\n\n");
  clearStdin();
  clearScreen();
}

void gameplay() { campMenu(); }

void prefaceMenu() {
  int validPrefaceMenuSelection = 0;
  while (!validPrefaceMenuSelection) {
    clearScreen();
    int selection = prompt("1. Read Preface\n2. Skip Preface\n\n> ");
    switch (selection) {
    case PREFACE_READ:
      validPrefaceMenuSelection = 1;
      preface();
      gameplay();
      return;
      break;
    case PREFACE_SKIP:
      validPrefaceMenuSelection = 1;
      gameplay();
      return;
    default:
      invalidInputMessage();
    }
  }
}

void mainMenu() {
  int validMainMenuSelection = 0;
  while (!validMainMenuSelection) {
    clearScreen();
    int selection =
        prompt("Halfway Across\n\n1. Begin Journal\n2. Go Home\n\n> ");
    switch (selection) {
    case MAIN_NEW_JOURNAL:
      validMainMenuSelection = 1;
      prefaceMenu();
      break;
    case MAIN_GO_HOME:
      validMainMenuSelection = 1;
      break;
    default:
      invalidInputMessage();
    }
  }
}