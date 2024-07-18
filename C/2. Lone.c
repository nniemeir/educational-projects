// Author: Jacob Niemeir <nniemeir@protonmail.com>
// A text-based survival game

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define NUM_OF_ITEMS 45

struct playerStats {
  int health;
  int energy;
  int hunger;
  int thirst;
  int temperature;
  int charm;
  int quickness;
  int day;
};

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

struct playerStats player = {100, 50, 100, 100, 0, 50, 20, 1};

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

void clearScreen() {
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}

void invalidInputMessage() {
  clearScreen();
  printf("My memory is failing me at the moment...\n");
  while (getchar() != '\n')
    ;
  clearScreen();
}

void listStats() {
  clearScreen();
  printf("Health: %d\n", player.health);
  printf("Energy: %d\n", player.energy);
  printf("Hunger: %d\n", player.hunger);
  printf("Thirst: %d\n", player.thirst);
  printf("Temperature: %d\n", player.temperature);
  printf("Charm: %d\n", player.charm);
  printf("Quickness: %d\n", player.quickness);
}

void warnings() {
  clearScreen();
  if (player.temperature < 20) {
    printf("I won't survive this cold much longer.\n");
  } else if (20 <= player.temperature && player.temperature < 50) {
    printf("The cold is becoming unbearable.\n");
  } else if (50 <= player.temperature && player.temperature < 70) {
    printf("The cold is starting to get to me.\n");
  }

  if (player.health < 20) {
    printf("My health is quickly deteriorating.\n");
  } else if (20 <= player.health && player.health < 50) {
    printf("I feel very ill.\n");
  } else if (50 <= player.health && player.health < 70) {
    printf("I feel a little under the weather.\n");
  }

  if (player.hunger < 20) {
    printf("I am starving.\n");
  } else if (20 <= player.hunger && player.hunger < 50) {
    printf("I need to eat something soon.\n");
  } else if (50 <= player.hunger && player.hunger < 70) {
    printf("I am starting to get hungry.\n");
  }

  if (player.thirst < 20) {
    printf("My throat is very dry.\n");
  } else if (20 <= player.thirst && player.thirst < 50) {
    printf("I need to drink something soon.\n");
  } else if (50 <= player.thirst && player.thirst < 70) {
    printf("I am starting to get thirsty.\n");
  }
  printf("\nI will persist...");
  while (getchar() != '\n')
    ;
}

int generateTemperature() {
  int dailyTemperature = rand() % (30 + 1);
  return dailyTemperature;
}

char *generateConditions() {
  float blizzardProbability = 5;
  float snowyProbability = 15;
  float cloudyProbability = 30;
  float clearProbability = 50;
  static char conditions[20];

  int prob = rand() % (100 + 1);
  if (prob <= 5) {
    strcpy(conditions, "snowed heavily");
  } else if (prob > 5 && prob <= 20) {
    strcpy(conditions, "snowed");
  } else if (prob > 20 && prob <= 50) {
    strcpy(conditions, "cloudy");
  } else if (prob > 50) {
    strcpy(conditions, "clear");
  } else {
    printf("There was an issue generating weather conditions");
    return NULL;
  }
  return conditions;
}

int findItemIndex(char *itemToUseName) {
  int itemIndex;
  int itemsIncrement;
  for (itemsIncrement = 0; itemsIncrement < NUM_OF_ITEMS; itemsIncrement++) {
    if (strcmp(inventory.items[itemsIncrement].name, itemToUseName) == 0) {
      itemIndex = itemsIncrement;
      return itemIndex;
    }
  }
  exit(1);
}

void useItem(int itemIndex) {
  clearScreen();
  if (strcmp(inventory.items[itemIndex].type, "food") == 0) {
    player.health = player.health + inventory.items[itemIndex].effects;
    inventory.items[itemIndex].amount = inventory.items[itemIndex].amount - 1;
  } else if (strcmp(inventory.items[itemIndex].type, "liquid") == 0) {
    player.thirst = player.thirst + inventory.items[2].effects;
    inventory.items[itemIndex].amount = inventory.items[itemIndex].amount - 1;
  } else if (strcmp(inventory.items[itemIndex].type, "clothing") == 0) {
    if (inventory.items[itemIndex].equipped == true) {
      printf("I took off my %s...\n", inventory.items[itemIndex].name);
      inventory.items[itemIndex].equipped = false;
       player.temperature =
          player.temperature - inventory.items[itemIndex].effects;
    } else {
      printf("I put on my %s...\n", inventory.items[itemIndex].name);
      inventory.items[itemIndex].equipped = true;
      player.temperature =
          player.temperature + inventory.items[itemIndex].effects;
    }
  } else if (strcmp(inventory.items[itemIndex].type, "tool") == 0) {
    printf("Tool usage not implemented yet");
  } else if (strcmp(inventory.items[itemIndex].type, "resource") == 0) {
    printf("Crafting not implemented yet");
  } else {
    printf("Invalid inventory entry");
  }
   while (getchar() != '\n')
    ;
}

void discardItem(int itemIndex) {
  int validDiscardAmount = 0;
  char discardAmount[5];
  while (!validDiscardAmount) {
    clearScreen();
    printf("I had %d, so I discarded...\n> ",
           inventory.items[itemIndex].amount);
    fgets(discardAmount, sizeof(discardAmount), stdin);
    discardAmount[strcspn(discardAmount, "\n")] = '\0';
    if (discardAmount[0] != '\0') {
      int discardAmountInt = atoi(discardAmount);
      if (discardAmountInt >= 0 &&
          discardAmountInt <= inventory.items[itemIndex].amount) {
        validDiscardAmount = 1;
        if (inventory.items[itemIndex].equipped == true) {
          useItem(itemIndex);
        }
        inventory.items[itemIndex].amount =
            inventory.items[itemIndex].amount - discardAmountInt;
      } else {
        printf("I didn't have enough to discard that many...\n");
        clearScreen();
      }
    }
  }
}

void inventoryMenu() {
  int exitedInventory = 0;
  while (!exitedInventory) {
    int validItemSelection = 0;
    char itemSelection[3];
    int itemSelectionInt = -1;
    char itemsPossessed[NUM_OF_ITEMS][30] = {};
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
      fgets(itemSelection, sizeof(itemSelection), stdin);
      itemSelection[strcspn(itemSelection, "\n")] = '\0';

      if (itemSelection[0] == '\0') {
        clearScreen();
        printf("I switched my focus to another matter...\n");
        return;
      }

      itemSelectionInt = atoi(itemSelection);

      if (itemSelectionInt < 0 || itemSelectionInt >= itemsPossessedSize) {
        invalidInputMessage();
      } else {
        int itemIndex = findItemIndex(itemsPossessed[itemSelectionInt]);
        validItemSelection = 1;
        char itemMenuSelection[3];
        int validMenuSelection = 0;
        while (!validMenuSelection) {
          clearScreen();
          printf("%s\n\n1. Use\n2. Craft\n3. Discard\n\n> ",
                 itemsPossessed[itemSelectionInt]);
          fgets(itemMenuSelection, sizeof(itemMenuSelection), stdin);
          itemMenuSelection[strcspn(itemMenuSelection, "\n")] = '\0';
          if (itemMenuSelection[0] == '\0') {
            clearScreen();
            printf("I chose to let it be...\n");
            while (getchar() != '\n')
              ;
            break;
          }

          int itemMenuSelectionInt = atoi(itemMenuSelection);

          switch (itemMenuSelectionInt) {
          case 1:
            useItem(itemIndex);
            validMenuSelection = 1;
            break;
          case 2:
            printf("Crafting not yet implemented...\n");
            while (getchar() != '\n')
              ;
            validMenuSelection = 1;
            break;
          case 3:
            discardItem(itemIndex);
            validMenuSelection = 1;
            break;
          default:
            invalidInputMessage();
          }
        }
      }
    }
  }
}

void huntingMenu() {
  clearScreen();
  printf("Hunting is not yet implemented...\n");
  while (getchar() != '\n')
    ;
  clearScreen();
  return;
}

void foragingMenu() {
  clearScreen();
  printf("Foraging is not yet implemented...\n");
  while (getchar() != '\n')
    ;
  clearScreen();
  return;
}

void fishingMenu() {
  clearScreen();
  printf("Fishing is not yet implemented...\n");
  while (getchar() != '\n')
    ;
  clearScreen();
  return;
}

void lakeMenu() {
  int validLakeDestination = 0;
  while (!validLakeDestination) {
    clearScreen();
    char lakeDestination[4];
    printf("After reaching the lake, I decided to...\n\n1. Fish\n2. "
           "Reflect\n3. Return to Camp\n\n> ");
    fgets(lakeDestination, sizeof(lakeDestination), stdin);
    lakeDestination[strcspn(lakeDestination, "\n")] = '\0';
    if (lakeDestination[0] == '\0') {
      clearScreen();
      printf("I realized that I still had something to do at camp before "
             "leaving...\n");
      while (getchar() != '\n')
        ;
      clearScreen();
      return;
    }
    int lakeDestinationInt = atoi(lakeDestination);
    switch (lakeDestinationInt) {
    case 1:
      fishingMenu();
      break;
    case 2:
      warnings();
      break;
    case 3:
      validLakeDestination = 1;
      break;
    default:
      invalidInputMessage();
    }
  }
}

void valleyMenu() {
  int validValleyDestination = 0;
  while (!validValleyDestination) {
    clearScreen();
    char valleyDestination[4];
    printf("After reaching the valley, I decided to...\n\n1. Gather plants\n2. "
           "Hunt\n3. Reflect\n4. Return to Camp\n\n> ");
    fgets(valleyDestination, sizeof(valleyDestination), stdin);
    valleyDestination[strcspn(valleyDestination, "\n")] = '\0';
    int valleyDestinationInt = atoi(valleyDestination);
    switch (valleyDestinationInt) {
    case 1:
      foragingMenu();
      break;
    case 2:
      huntingMenu();
      break;
    case 3:
      warnings();
      break;
    case 4:
      validValleyDestination = 1;
      break;
    default:
      invalidInputMessage();
    }
  }
}

void travelMenu() {
  int validDestination = 0;
  while (!validDestination) {
    clearScreen();
    char destination[4];
    printf("I walked to...\n\n1. The Lake\n2. The Valley\n\n> ");
    fgets(destination, sizeof(destination), stdin);
    destination[strcspn(destination, "\n")] = '\0';
    if (destination[0] == '\0') {
      clearScreen();
      printf("I realized that I still had something to do at camp before "
             "leaving...\n");
      while (getchar() != '\n')
        ;
      clearScreen();
      return;
    }
    int destinationInt = atoi(destination);
    switch (destinationInt) {
    case 1:
      validDestination = 1;
      lakeMenu();
      break;
    case 2:
      validDestination = 1;
      valleyMenu();
      break;
    default:
      invalidInputMessage();
    }
  }
}

// Calculate whether to lower player's stats at end of day
void advanceDay() { player.day = player.day + 1; 
}

int homeMenu() {
  // TO DO - PUT IN TEMPERATURE INITIALIZATION BASED ON CLOTHES FOR DAY 1
  char homeSelection[3];
  int leftHome = 0;
  int dailyTemperature = generateTemperature();
  char *conditions = generateConditions();
  while (!leftHome) {
    clearScreen();
    printf("Day %d\n\n", player.day);
    printf("It was %d degrees and %s today.\n", dailyTemperature, conditions);
    printf("I decided to...\n\n");
    printf("1. Leave Camp\n2. Examine My Belongings\n3. Reflect\n4. Sleep\n5. "
           "Give Up\n\n> ");
    fgets(homeSelection, sizeof(homeSelection), stdin);
    homeSelection[strcspn(homeSelection, "\n")] = '\0';
    if (strcmp(homeSelection, "1") == 0) {
      if (strcmp(conditions, "snowed heavily") == 0) {
        clearScreen();
        printf("It was snowing too heavily for me to travel...");
        while (getchar() != '\n')
          ;
        clearScreen();
      } else {
        travelMenu();
        int leftHome = 1;
      }
    } else if (strcmp(homeSelection, "2") == 0) {
      inventoryMenu();
      while (getchar() != '\n')
        ;
    } else if (strcmp(homeSelection, "3") == 0) {
      warnings();
    } else if (strcmp(homeSelection, "4") == 0) {
      advanceDay();
      dailyTemperature = generateTemperature();
      char *conditions = generateConditions();
      if (player.health <= 0) {
        clearScreen();
        printf("This is my last entry, my health is failing me. Whoever finds "
               "this journal is welcome to whatever is left in my camp...");
        while (getchar() != '\n')
          ;
        leftHome = 1;
      }
    } else if (strcmp(homeSelection, "5") == 0) {
      clearScreen();
      printf("The journal ends here...\n");
      while (getchar() != '\n')
        ;
      break;
    } else if (strcmp(homeSelection, "6") == 0) {
      listStats();
      while (getchar() != '\n')
        ;
    } else {
      invalidInputMessage();
    }
  }
  return 0;
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
  while (getchar() != '\n')
    ;
  clearScreen();
}

void gameplay() { homeMenu(); }

void mainMenu() {
  clearScreen();
  char mainSelection[4];
  char prefaceSelection[4];
  int validMainSelection = 0;
  int validPrefaceSelection = 0;
  while (!validMainSelection) {
    printf("L O N E\n\n1. New Journal\n2. Continue Journal\n3. Go Home\n\n> ");
    fgets(mainSelection, sizeof(mainSelection), stdin);
    mainSelection[strcspn(mainSelection, "\n")] = '\0';
    if (strcmp(mainSelection, "1") == 0) {
      while (!validPrefaceSelection) {
        clearScreen();
        printf("1. Read Preface\n2. Skip Preface\n\n> ");
        fgets(prefaceSelection, sizeof(prefaceSelection), stdin);
        prefaceSelection[strcspn(prefaceSelection, "\n")] = '\0';
        if (strcmp(prefaceSelection, "1") == 0) {
          validPrefaceSelection = 1;
          preface();
          gameplay();
          return;
        } else if (strcmp(prefaceSelection, "2") == 0) {
          validPrefaceSelection = 1;
          gameplay();
          return;
        } else {
          invalidInputMessage();
        }
      }
    } else if (strcmp(mainSelection, "2") == 0) {
      clearScreen();
      printf("Saving not yet implemented...\n");
      while (getchar() != '\n')
        ;
      clearScreen();
    } else if (strcmp(mainSelection, "3") == 0) {
      return;
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
