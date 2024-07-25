#include "lone.h"

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

typedef enum { ITEM_USE = 1, ITEM_CRAFT, ITEM_DISCARD } ItemMenuOption;


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

int inputIsNumeric(const char *input) {
  while (*input) {
    if (!isdigit(*input))
      return 0;
    input++;
  }
  return 1;
}

void itemMenu(char *selectedItemName) {
  int selectedItemIndex = findItemIndex(selectedItemName);
  char itemMenuSelection[PROMPT_SIZE];
  int validItemMenuSelection = 0;

  while (!validItemMenuSelection) {
    clearScreen();
    printf("%s\n\n1. Use\n2. Craft\n3. Discard\n\n> ", selectedItemName);

    if (!fgets(itemMenuSelection, sizeof(itemMenuSelection), stdin)) {
      fprintf(stderr, "Error reading input.\n");
      continue;
    }
    itemMenuSelection[strcspn(itemMenuSelection, "\n")] = '\0';

    if (itemMenuSelection[0] == '\0') {
      clearScreen();
      printf("I chose to let it be...\n");
      clearStdin();
      break;
    }

    if (inputIsNumeric(itemMenuSelection)) {
      char *endptr;
      long selection = strtol(itemMenuSelection, &endptr, 10);
      if (endptr == itemMenuSelection || *endptr != '\0') {
        invalidInputMessage();
        continue;
      }
      switch (selection) {
      case ITEM_USE:
        useItem(selectedItemIndex);
        validItemMenuSelection = 1;
        break;
      case ITEM_CRAFT:
        printf("Crafting not yet implemented...\n");
        clearStdin();
        validItemMenuSelection = 1;
        break;
      case ITEM_DISCARD:
        discardItem(selectedItemIndex);
        validItemMenuSelection = 1;
        break;
      default:
        invalidInputMessage();
      }
    } else {
      invalidInputMessage();
    }
  }
}

void inventoryMenu() {
  int exitedInventory = 0;
  while (!exitedInventory) {
    int validItemSelection = 0;
    char itemSelection[PROMPT_SIZE];
    int selection = -1;
    char itemsPossessed[NUM_OF_ITEMS][ITEM_NAME_SIZE] = {};
    while (!validItemSelection) {
      clearScreen();
      int itemsPossessedSize = 0;
      printf("I had a variety of items, I inspected my...\n\n");
      for (int itemsIncrement = 0; itemsIncrement < NUM_OF_ITEMS;
           itemsIncrement++) {
        if (inventory.items[itemsIncrement].amount != 0) {
          strcpy(itemsPossessed[itemsPossessedSize],
                 inventory.items[itemsIncrement].name);
          printf("%d. %s\n", itemsPossessedSize,
                 inventory.items[itemsIncrement].name);
          itemsPossessedSize++;
        }
      }
      printf("\n> ");
      if (!fgets(itemSelection, sizeof(itemSelection), stdin)) {
        fprintf(stderr, "Error reading input.\n");
        continue;
      }
      itemSelection[strcspn(itemSelection, "\n")] = '\0';
      if (itemSelection[0] == '\0') {
        clearScreen();
        printf("I switched my focus to another matter...\n");
        return;
      }

      if (inputIsNumeric(itemSelection)) {
        char *endptr;
        long selection = strtol(itemSelection, &endptr, 10);
        if (endptr == itemSelection || *endptr != '\0') {
          invalidInputMessage();
          continue;
        }
        if (selection < 0 || selection >= itemsPossessedSize) {
          invalidInputMessage();
        } else {
          validItemSelection = 1;
          itemMenu(itemsPossessed[selection]);
        }
      }
    }
  }
}

void huntingMenu() {
  clearScreen();
  int stoppedHunting = 0;
  while (!stoppedHunting) {
    char *animal = generateAnimal();
    printf("I spotted a %s\n", animal);
    while (getchar() != '\n')
      ;
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
  int validLakeMenuSelection = 0;
  char lakeMenuSelection[PROMPT_SIZE];
  while (!validLakeMenuSelection) {
    clearScreen();
    printf("After reaching the lake, I decided to...\n\n1. Fish\n2. "
           "Reflect\n3. Return to Camp\n\n> ");
    if (!fgets(lakeMenuSelection, sizeof(lakeMenuSelection), stdin)) {
      fprintf(stderr, "Error reading input.\n");
      continue;
    }
    lakeMenuSelection[strcspn(lakeMenuSelection, "\n")] = '\0';
    if (inputIsNumeric(lakeMenuSelection)) {
      char *endptr;
      long selection = strtol(lakeMenuSelection, &endptr, 10);
      if (endptr == lakeMenuSelection || *endptr != '\0') {
        invalidInputMessage();
        continue;
      }
      switch (selection) {
      case LAKE_FISH:
        validLakeMenuSelection = 1;
        fishingMenu();
        break;
      case LAKE_REFLECT:
        warnings();
        break;
      case LAKE_LEAVE:
        validLakeMenuSelection = 1;
        break;
      default:
        invalidInputMessage();
      }
    } else {
      invalidInputMessage();
    }
  }
}

void valleyMenu() {
  int validValleyMenuSelection = 0;
  char valleyMenuSelection[PROMPT_SIZE];
  while (!validValleyMenuSelection) {
    clearScreen();
    printf("After reaching the valley, I decided to...\n\n1. Gather plants\n2. "
           "Hunt\n3. Reflect\n4. Return to Camp\n\n> ");
    if (!fgets(valleyMenuSelection, sizeof(valleyMenuSelection), stdin)) {
      fprintf(stderr, "Error reading input.\n");
      continue;
    }
    valleyMenuSelection[strcspn(valleyMenuSelection, "\n")] = '\0';
    if (inputIsNumeric(valleyMenuSelection)) {
      char *endptr;
      long selection = strtol(valleyMenuSelection, &endptr, 10);
      if (endptr == valleyMenuSelection || *endptr != '\0') {
        invalidInputMessage();
        continue;
      }
      switch (selection) {
      case VALLEY_GATHER_PLANTS:
        validValleyMenuSelection = 1;
        foragingMenu();
        break;
      case VALLEY_HUNT:
        validValleyMenuSelection = 1;
        huntingMenu();
        break;
      case VALLEY_REFLECT:
        warnings();
        break;
      case VALLEY_LEAVE:
        validValleyMenuSelection = 1;
        break;
      default:
        invalidInputMessage();
      }
    } else {
      invalidInputMessage();
    }
  }
}

void travelMenu() {
  int validTravelMenuSelection = 0;
  char travelMenuSelection[PROMPT_SIZE];
  while (!validTravelMenuSelection) {
    clearScreen();
    printf("I walked to...\n\n1. The Lake\n2. The Valley\n\n> ");
    if (!fgets(travelMenuSelection, sizeof(travelMenuSelection), stdin)) {
      fprintf(stderr, "Error reading input.\n");
      continue;
    }
    travelMenuSelection[strcspn(travelMenuSelection, "\n")] = '\0';
    if (inputIsNumeric(travelMenuSelection)) {
      char *endptr;
      long selection = strtol(travelMenuSelection, &endptr, 10);
      if (endptr == travelMenuSelection || *endptr != '\0') {
        invalidInputMessage();
        continue;
      }
      player.energy = 0;
      switch (selection) {
      case TRAVEL_LAKE:
        validTravelMenuSelection = 1;
        lakeMenu();
        break;
      case TRAVEL_VALLEY:
        validTravelMenuSelection = 1;
        valleyMenu();
        break;
      default:
        invalidInputMessage();
      }
    } else {
      invalidInputMessage();
    }
  }
}


void campMenu() {
  char campMenuSelection[PROMPT_SIZE];
  int validCampMenuSelection = 0;
  while (!validCampMenuSelection) {
    clearScreen();
    printf("Day %d\n\n", world.day);
    printf("It was %d degrees and %s today.\n", world.currentTemperature,
           world.currentWeather);
    printf("I decided to...\n\n");
    printf("1. Leave Camp\n2. Examine My Belongings\n3. Reflect\n4. Sleep\n5. "
           "Give Up\n\n> ");
    if (!fgets(campMenuSelection, sizeof(campMenuSelection), stdin)) {
      fprintf(stderr, "Error reading input.\n");
      continue;
    }
    campMenuSelection[strcspn(campMenuSelection, "\n")] = '\0';

    if (inputIsNumeric(campMenuSelection)) {
      char *endptr;
      long selection = strtol(campMenuSelection, &endptr, 10);
      if (endptr == campMenuSelection || *endptr != '\0') {
        invalidInputMessage();
        continue;
      }
      switch (selection) {
      case CAMP_LEAVE:
        if (strcmp(world.currentWeather, "snowed heavily") == 0) {
          clearScreen();
          printf("It was snowing too heavily for me to travel...");
          clearStdin();
          clearScreen();
        } else if (player.energy == 0) {
          clearScreen();
          printf("I didn't have the energy to travel anywhere else...");
          clearStdin();
          clearScreen();
        } else {
          travelMenu();
          int validCampMenuSelection = 1;
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
        validCampMenuSelection = 1;
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
    } else {
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
  clearScreen();
  char prefaceMenuSelection[PROMPT_SIZE];
  int validPrefaceMenuSelection = 0;
  while (!validPrefaceMenuSelection) {
    printf("1. Read Preface\n2. Skip Preface\n\n> ");
    if (!fgets(prefaceMenuSelection, sizeof(prefaceMenuSelection), stdin)) {
      fprintf(stderr, "Error reading input.\n");
      continue;
    }
    prefaceMenuSelection[strcspn(prefaceMenuSelection, "\n")] = '\0';

    if (inputIsNumeric(prefaceMenuSelection)) {
      char *endptr;
      long selection = strtol(prefaceMenuSelection, &endptr, 10);
      if (endptr == prefaceMenuSelection || *endptr != '\0') {
        invalidInputMessage();
        continue;
      }
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
    } else {
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
  clearScreen();
  char mainMenuSelection[PROMPT_SIZE];
  int validMainMenuSelection = 0;
  while (!validMainMenuSelection) {
    printf("L O N E\n\n1. New Journal\n2. Continue Journal\n3. Go Home\n\n> ");
    if (!fgets(mainMenuSelection, sizeof(mainMenuSelection), stdin)) {
      fprintf(stderr, "Error reading input.\n");
      continue;
    }
    mainMenuSelection[strcspn(mainMenuSelection, "\n")] = '\0';

    if (inputIsNumeric(mainMenuSelection)) {
      char *endptr;
      long selection = strtol(mainMenuSelection, &endptr, 10);
      if (endptr == mainMenuSelection || *endptr != '\0') {
        invalidInputMessage();
        continue;
      }
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
    } else {
      invalidInputMessage();
    }
  }
}