#include "lone.h"

#define INVENTORY_ARRAY_OFFSET 1

typedef enum {
  MAIN_NEW_JOURNAL = 1,
  MAIN_CONTINUE_JOURNAL,
  MAIN_GO_HOME
} MainMenuOption;

typedef enum { PREFACE_READ = 1, PREFACE_SKIP } PrefaceMenuOption;

typedef enum {
  CAMP_LEAVE = 1,
  SHOW_INVENTORY,
  CAMP_REFLECT,
  CAMP_ADVANCE_DAY,
  CAMP_GIVE_UP,
  CAMP_DEBUG_STATS
} CampMenuOption;

typedef enum { TRAVEL_LAKE = 1, TRAVEL_VALLEY } TravelMenuOption;

typedef enum {
  VALLEY_GATHER_PLANTS = 1,
  VALLEY_HUNT,
  VALLEY_REFLECT,
  VALLEY_LEAVE
} ValleyMenuOption;

typedef enum { LAKE_FISH = 1, LAKE_REFLECT, LAKE_LEAVE } LakeMenuOption;

typedef enum {
  ITEM_USE = 1,
  ITEM_CRAFT,
  ITEM_DISCARD,
  ITEM_EXIT
} ItemMenuOption;

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

// Debug tool for testing changes to stats
void listStats() {
  clearScreen();
  printf("Health: %d\n", player.health);
  printf("Energy: %d\n", player.energy);
  printf("Hunger: %d\n", player.hunger);
  printf("Thirst: %d\n", player.thirst);
  printf("Temperature: %d\n", player.warmth);
  printf("Charm: %d\n", player.charm);
}

void craftingMenu() {
  clearScreen();
  printf("Crafting is not yet implemented...\n");
  clearStdin();
  clearScreen();
}

void itemMenu(char *selectedItemName) {
  int selectedItemIndex = findItemIndex(selectedItemName);
  int validItemMenuSelection = 0;
  while (!validItemMenuSelection) {
    clearScreen();
    printf("%s\n\n", selectedItemName);
    int selection = prompt("1. Use\n2. Craft\n3. Discard\n4. Exit\n\n> ");
    switch (selection) {
    case ITEM_USE:
      useItem(selectedItemIndex);
      clearStdin();
      validItemMenuSelection = 1;
      break;
    case ITEM_CRAFT:
      craftingMenu();
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

struct inventoryValues values = {{"nothing"}, 0, 0, 0};
char *generateInventoryPrompt(char *generatedInventoryPrompt) {
  strcpy(generatedInventoryPrompt,
         "I had a variety of items, I inspected my...\n\n");
  char buffer[100];
  for (int itemsIncrement = 0; itemsIncrement < NUM_OF_ITEMS;
       itemsIncrement++) {
    if (inventory.items[itemsIncrement].amount != 0) {
      strcpy(values.itemsPossessed[values.itemCount],
             inventory.items[itemsIncrement].name);
      snprintf(buffer, sizeof(buffer), "%d", values.itemCount + INVENTORY_ARRAY_OFFSET);
      strcat(generatedInventoryPrompt, buffer);
      strcat(generatedInventoryPrompt, ". ");
      strcat(generatedInventoryPrompt, inventory.items[itemsIncrement].name);
      strcat(generatedInventoryPrompt, "\n");
      values.itemCount++;
    }
  }
  snprintf(buffer, sizeof(buffer), "%d", values.itemCount + INVENTORY_ARRAY_OFFSET);
  strcat(generatedInventoryPrompt, buffer);
  strcat(generatedInventoryPrompt, ". Exit\n\n> ");
  return generatedInventoryPrompt;
}

void resetInventoryValues() {
  for (int itemsPossessedIncrement = 0; itemsPossessedIncrement < NUM_OF_ITEMS;
       itemsPossessedIncrement++) {
    memset(values.itemsPossessed[itemsPossessedIncrement], '\0',
           ITEM_NAME_SIZE);
  }
  values.itemCount = 0;
  values.numOfItemsPossessed = 0;
  values.selection = 0;
}

void inventoryMenu() {
  int validInventorySelection = 0;
  while (!validInventorySelection) {
    resetInventoryValues();
    clearScreen();
    char generatedInventoryPrompt[PROMPT_SIZE];
    char *inventoryPrompt = generateInventoryPrompt(generatedInventoryPrompt);
    values.selection = prompt(inventoryPrompt);
    // Fix offset
    int fixedOffsetSelection = values.selection - 1;
    if (values.selection > 0 && values.selection <= values.itemCount) {
      itemMenu(values.itemsPossessed[fixedOffsetSelection]);
    } else if (values.selection == values.itemCount + INVENTORY_ARRAY_OFFSET) {
      validInventorySelection = 1;
      clearScreen();
      printf("I switched my focus to another matter...\n");
      return;
    } else {
      invalidInputMessage();
    }
  }
}

void huntingMenu() {
  clearScreen();
  int stoppedHunting = 0;
  while (!stoppedHunting) {
    char *animal = generateAnimal();
    printf("I spotted a %s\n", animal);
    clearStdin();
    clearScreen();
    stoppedHunting = 1;
    return;
  }
}

void foragingMenu() {
  clearScreen();
  printf("Foraging is not yet implemented...\n");
  clearStdin();
  clearScreen();
  return;
}

void fishingMenu() {
  clearScreen();
  printf("Fishing is not yet implemented...\n");
  clearStdin();
  clearScreen();
  return;
}

void lakeMenu() {
  int leftLake = 0;
  while (!leftLake) {
    clearScreen();
    int selection =
        prompt("After reaching the lake, I decided to...\n\n1. Fish\n2. "
               "Reflect\n3. Return to Camp\n\n> ");
    switch (selection) {
    case LAKE_FISH:
      leftLake = 1;
      fishingMenu();
      break;
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
    int selection =
        prompt("After reaching the valley, I decided to...\n\n1. Gather "
               "plants\n2. Hunt\n3. Reflect\n4. Return to Camp\n\n> ");
    switch (selection) {
    case VALLEY_GATHER_PLANTS:
      leftValley = 1;
      foragingMenu();
      break;
    case VALLEY_HUNT:
      leftValley = 1;
      huntingMenu();
      break;
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
    int selection =
        prompt("I walked to...\n\n1. The Lake\n2. The Valley\n\n> ");
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
    default:
      invalidInputMessage();
    }
  }
}

void campMenu() {
  int leftCamp = 0;
  while (!leftCamp) {
    clearScreen();
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
        int leftCamp = 1;
      }
      break;
    case SHOW_INVENTORY:
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
      leftCamp = 1;
      clearScreen();
      printf("The journal ends here...\n");
      clearStdin();
      break;
    case CAMP_DEBUG_STATS:
      listStats();
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

void saveMenu() {
  clearScreen();
  printf("Saving not yet implemented...\n");
  clearStdin();
  clearScreen();
}

void mainMenu() {
  int validMainMenuSelection = 0;
  while (!validMainMenuSelection) {
    clearScreen();
    int selection = prompt(
        "L O N E\n\n1. New Journal\n2. Continue Journal\n3. Go Home\n\n> ");
    switch (selection) {
    case MAIN_NEW_JOURNAL:
      validMainMenuSelection = 1;
      prefaceMenu();
      break;
    case MAIN_CONTINUE_JOURNAL:
      validMainMenuSelection = 1;
      saveMenu();
      break;
    case MAIN_GO_HOME:
      validMainMenuSelection = 1;
      break;
    default:
      invalidInputMessage();
    }
  }
}