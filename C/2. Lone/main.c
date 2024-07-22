// Author: Jacob Niemeir <nniemeir@protonmail.com>
// A text-based survival game
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define NUM_OF_ITEMS 45
#define ITEM_NAME_SIZE 255
#define PROMPT_SIZE 100
#define LOW_STAT_THRESHOLD 20
#define MEDIUM_STAT_THRESHOLD 50
#define HIGH_STAT_THRESHOLD 70

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

struct worldStats {
  int day;
  int currentTemperature;
  char currentWeather[255];
};

struct worldStats world = {1, 32, "clear"};

struct playerStats {
  int health;
  int energy;
  int hunger;
  int thirst;
  int warmth;
  int charm;
};

struct playerStats player = {100, 1, 100, 100, 50, 50};

struct item {
  char name[50];
  int amount;
  bool equipped;
  int effects;
  char type[50];
};

struct playerInventory {
  struct item items[NUM_OF_ITEMS];
};

struct playerInventory inventory = {{
    {"Badger Meat", 0, false, 20, "food"},
    {"Bear Meat", 0, false, 100, "food"},
    {"Deer Meat", 0, false, 40, "food"},
    {"Fox Meat", 0, false, 20, "food"},
    {"Rabbit Meat", 0, false, 10, "food"},
    {"Squirrel Meat", 0, false, 10, "food"},
    {"Barberry", 0, false, 2, "food"},
    {"Bindweed", 0, false, -2, "food"},
    {"Buckthorn", 0, false, -5, "food"},
    {"Chokecherry", 0, false, 3, "food"},
    {"Common Burdock", 0, false, 5, "food"},
    {"Dandelion", 0, false, 5, "food"},
    {"Mountain Deathcamas", 0, false, -100, "food"},
    {"Oregon Grape", 0, false, 2, "food"},
    {"Purslane", 0, false, 2, "food"},
    {"Stinging Nettle", 0, false, 10, "food"},
    {"Wild Onion", 0, false, 2, "food"},
    {"Wild Strawberry", 0, false, 2, "food"},
    {"Bearskin Cloak", 0, false, 50, "clothing"},
    {"Fox Skin Gloves", 0, false, 7, "clothing"},
    {"Leather Gloves", 1, true, 5, "clothing"},
    {"Rabbit Skin Gloves", 0, false, 8, "clothing"},
    {"Badger Hat", 0, false, 7, "clothing"},
    {"Felt Hat", 1, true, 5, "clothing"},
    {"Fox Hat", 0, false, 8, "clothing"},
    {"Raccoon Hat", 0, false, 7, "clothing"},
    {"Heavy Cotton Shirt", 1, true, 10, "clothing"},
    {"Heavy Cotton Trousers", 1, true, 20, "clothing"},
    {"Heavy Wool Vest", 0, false, 20, "clothing"},
    {"Moccasins", 1, true, 10, "clothing"},
    {"Bearskin Boots", 0, false, 30, "clothing"},
    {"Dearskin Boots", 0, false, 20, "clothing"},
    {"Rabbit Boots", 0, false, 15, "clothing"},
    {"Axe", 0, false, 0, "tool"},
    {"Kettle", 0, false, 0, "tool"},
    {"Knife", 0, false, 0, "tool"},
    {"Purified Water", 0, false, 50, "liquid"},
    {"River Water", 0, false, 25, "liquid"},
    {"Wood", 0, false, 0, "resource"},
    {"Badger pelt", 0, false, 0, "resource"},
    {"Bear pelt", 0, false, 0, "resource"},
    {"Deer pelt", 0, false, 0, "resource"},
    {"Fox pelt", 0, false, 0, "resource"},
    {"Rabbit pelt", 0, false, 0, "resource"},
    {"Squirrel pelt", 0, false, 0, "resource"},
}};

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

// Gives player vague indication of stats if they select "Reflect" option
void warnings() {
  clearScreen();
  if (player.warmth < LOW_STAT_THRESHOLD) {
    printf("I won't survive this cold much longer.\n");
  } else if (LOW_STAT_THRESHOLD <= player.warmth &&
             player.warmth < MEDIUM_STAT_THRESHOLD) {
    printf("The cold is becoming unbearable.\n");
  } else if (MEDIUM_STAT_THRESHOLD <= player.warmth &&
             player.warmth < HIGH_STAT_THRESHOLD) {
    printf("The cold is starting to get to me.\n");
  }

  if (player.health < LOW_STAT_THRESHOLD) {
    printf("My health is quickly deteriorating.\n");
  } else if (LOW_STAT_THRESHOLD <= player.health &&
             player.health < MEDIUM_STAT_THRESHOLD) {
    printf("I feel very ill.\n");
  } else if (MEDIUM_STAT_THRESHOLD <= player.health &&
             player.health < HIGH_STAT_THRESHOLD) {
    printf("I feel a little under the currentWeather.\n");
  }

  if (player.hunger < LOW_STAT_THRESHOLD) {
    printf("I am starving.\n");
  } else if (LOW_STAT_THRESHOLD <= player.hunger &&
             player.hunger < MEDIUM_STAT_THRESHOLD) {
    printf("I need to eat something soon.\n");
  } else if (MEDIUM_STAT_THRESHOLD <= player.hunger &&
             player.hunger < HIGH_STAT_THRESHOLD) {
    printf("I am starting to get hungry.\n");
  }

  if (player.thirst < LOW_STAT_THRESHOLD) {
    printf("My throat is very dry.\n");
  } else if (LOW_STAT_THRESHOLD <= player.thirst &&
             player.thirst < MEDIUM_STAT_THRESHOLD) {
    printf("I need to drink something soon.\n");
  } else if (MEDIUM_STAT_THRESHOLD <= player.thirst &&
             player.thirst < HIGH_STAT_THRESHOLD) {
    printf("I am starting to get thirsty.\n");
  }
  printf("\nI will persist...");
  clearStdin();
}

// A given day's temperature has an effect on
// how high the temperature stat must be to avoid losing health
int generateTemperature() {
  int currentTemperature = rand() % (30 + 1);
  return currentTemperature;
}

// A given day's currentWeather currentWeather have an effect on
// what actions the player can do and how likely they are to succeed at those
// they can
char *generateWeather() {
  float blizzardProb = 5;
  float snowyProb = 20;
  float cloudyProb = 50;
  static char currentWeather[20];

  int prob = rand() % 100;
  if (prob <= blizzardProb) {
    strcpy(currentWeather, "snowed heavily");
  } else if (prob > blizzardProb && prob <= snowyProb) {
    strcpy(currentWeather, "snowed");
  } else if (prob > snowyProb && prob <= cloudyProb) {
    strcpy(currentWeather, "cloudy");
  } else if (prob > cloudyProb) {
    strcpy(currentWeather, "clear");
  } else {
    printf("There was an issue generating currentWeather currentWeather");
    return NULL;
  }
  return currentWeather;
}

// Different scenarios are triggered based on the animal encountered in a
// hunting encounter
char *generateAnimal() {
  float badgerProb = 5;
  float bearProb = 30;
  float deerProb = 70;
  float foxProb = 75;
  float rabbitProb = 90;
  static char animal[20];

  int prob = rand() % (100 + 1);
  if (prob <= badgerProb) {
    strcpy(animal, "badger");
  } else if (prob > badgerProb && prob <= bearProb) {
    strcpy(animal, "bear");
  } else if (prob > bearProb && prob <= deerProb) {
    strcpy(animal, "deer");
  } else if (prob > deerProb && prob <= foxProb) {
    strcpy(animal, "fox");
  } else if (prob > foxProb && prob <= rabbitProb) {
    strcpy(animal, "rabbit");
  } else if (prob > rabbitProb) {
    strcpy(animal, "squirrel");
  } else {
    printf("There was an issue deciding what animal type should be "
           "encountered...");
    return NULL;
  }
  return animal;
}

// The index of the item selected by the player in inventoryMenu is needed to
// modify its members
int findItemIndex(char *selectedItemName) {
  int selectedItemIndex;
  int itemsIncrement;
  for (itemsIncrement = 0; itemsIncrement < NUM_OF_ITEMS; itemsIncrement++) {
    if (strcmp(inventory.items[itemsIncrement].name, selectedItemName) == 0) {
      selectedItemIndex = itemsIncrement;
      return selectedItemIndex;
    }
  }
  exit(1);
}

// The stat modified depends on the type of the item.
// Using a consumable subtracts one of it from your inventory
// However, using a clothing item instead equips/unequips it
void useItem(int selectedItemIndex) {
  clearScreen();
  if (strcmp(inventory.items[selectedItemIndex].type, "food") == 0) {
    player.health = player.health + inventory.items[selectedItemIndex].effects;
    inventory.items[selectedItemIndex].amount =
        inventory.items[selectedItemIndex].amount - 1;
  } else if (strcmp(inventory.items[selectedItemIndex].type, "liquid") == 0) {
    player.thirst = player.thirst + inventory.items[2].effects;
    inventory.items[selectedItemIndex].amount =
        inventory.items[selectedItemIndex].amount - 1;
  } else if (strcmp(inventory.items[selectedItemIndex].type, "clothing") == 0) {
    if (inventory.items[selectedItemIndex].equipped == 1) {
      printf("I took off my %s...\n", inventory.items[selectedItemIndex].name);
      inventory.items[selectedItemIndex].equipped = 0;
      player.warmth =
          player.warmth - inventory.items[selectedItemIndex].effects;
    } else {
      printf("I put on my %s...\n", inventory.items[selectedItemIndex].name);
      inventory.items[selectedItemIndex].equipped = 1;
      player.warmth =
          player.warmth + inventory.items[selectedItemIndex].effects;
    }
  } else if (strcmp(inventory.items[selectedItemIndex].type, "tool") == 0) {
    printf("Tool usage not implemented yet");
  } else if (strcmp(inventory.items[selectedItemIndex].type, "resource") == 0) {
    printf("Crafting not implemented yet");
  } else {
    printf("Invalid inventory entry");
  }
  clearStdin();
}

// Removes x of an item from the inventory if x is <= the amount currently
// possessed
void discardItem(int selectedItemIndex) {
  int validDiscardAmount = 0;
  char discardAmount[PROMPT_SIZE];
  while (!validDiscardAmount) {
    clearScreen();
    printf("I had %d, so I discarded...\n> ",
           inventory.items[selectedItemIndex].amount);

    if (!fgets(discardAmount, sizeof(discardAmount), stdin)) {
      fprintf(stderr, "Error reading input.\n");
      continue;
    }
    discardAmount[strcspn(discardAmount, "\n")] = '\0';

    if (inputIsNumeric(discardAmount)) {
      char *endptr;
      long selection = strtol(discardAmount, &endptr, 10);
      if (endptr == discardAmount || *endptr != '\0') {
        invalidInputMessage();
        continue;
      }
      discardAmount[strcspn(discardAmount, "\n")] = '\0';
      if (discardAmount[0] != '\0') {
        int discardAmountInt = atoi(discardAmount);
        if (discardAmountInt >= 0 &&
            discardAmountInt <= inventory.items[selectedItemIndex].amount) {
          validDiscardAmount = 1;
          if (inventory.items[selectedItemIndex].equipped == 1 &&
              discardAmountInt > 0) {
            useItem(selectedItemIndex);
          }
          inventory.items[selectedItemIndex].amount =
              inventory.items[selectedItemIndex].amount - discardAmountInt;
        } else {
          printf("I didn't have enough to discard that many...\n");
          clearScreen();
        }
      }
    } else {
      invalidInputMessage();
    }
  }
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

void advanceDay() {
  if (player.health <= 0) {
    clearScreen();
    printf("This is my last entry, my health is failing me. Whoever finds "
           "this journal is welcome to whatever is left in my camp...");
    clearStdin();
    clearScreen();
    return;
  }
  player.energy = 1;
  world.day = world.day + 1;
  world.currentTemperature = generateTemperature();
  memset(world.currentWeather, 0, sizeof(world.currentWeather));
  strcpy(world.currentWeather, generateWeather());
  return;
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

int main() {
  srand(time(NULL));
  mainMenu();
  return EXIT_SUCCESS;
}
