#include "../include/halfway_across.h"

typedef enum { MAIN_NEW_JOURNAL = 1, MAIN_GO_HOME } MainMenuOption;

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

void dailyMsg() {
  printf("San Juan Mountains, 1893\n");
  printf("Day %d\n\n", world.day);
  printf("It was %d degrees and %s today.\n", world.currentTemperature,
         world.currentWeather);
}

void deathMsg() {
  clearScreen();
  printf("This is my last entry, my health is failing me. Whoever finds "
         "this journal is welcome to whatever is left in my camp...");
  clearStdin();
  clearScreen();
}

void invalidInputMsg() {
  clearScreen();
  printf("My memory is failing me at the moment...\n");
  clearStdin();
  clearScreen();
}

void itemMenu(char *selectedItemName) {
  int selectedItemIndex = findItemIndex(selectedItemName);
  int validItemMenuSelection = 0;
  while (!validItemMenuSelection) {
    clearScreen();
    printf("%s\nQuantity: %d\nEquipped: %d\n\n", selectedItemName,
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
      invalidInputMsg();
    }
  }
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
      invalidInputMsg();
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
      invalidInputMsg();
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
      invalidInputMsg();
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
      invalidInputMsg();
    }
  }
}

void campMenu() {
  int gaveUp = 0;
  while (!gaveUp) {
    clearScreen();
    if (player.health <= 0) {
      deathMsg();
      return;
    }
    dailyMsg();
    int selection = prompt("I decided to...\n\n1. Leave Camp\n2. Examine My "
                           "Belongings\n3. Reflect\n4. Sleep\n5. "
                           "Give Up\n\n> ");
    switch (selection) {
    case CAMP_LEAVE:
      travelChecks();
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
      invalidInputMsg();
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
      campMenu();
      break;
    case MAIN_GO_HOME:
      validMainMenuSelection = 1;
      break;
    default:
      invalidInputMsg();
    }
  }
}