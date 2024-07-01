#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct playerStats {
  int health;
  int energy;
  int hunger;
  int thirst;
  int temperature;
  int charm;
  int quickness;
};

struct playerInventory {
  // Food
  char *food[18][20];
  int foodPossessed[18];
  int foodEffects[18];
  // Clothing
  char *clothing[18][50];
  int clothingPossessed[18];
  bool clothingEquipped[18];
  int clothingEffects[18];
  // Tools
  char *tools[6][20];
  int toolsPossessed[6];
  int toolsCondition[6];
  bool toolsEquipped[6];
  // Resources
  char *resources[3][15];
  int resourcesPossessed[6];
  // Pelts
  char *pelts[6][9];
  int peltsPossessed[6];
};

struct playerStats player = {100, 50, 100, 100, 50, 50, 20};

struct playerInventory inventory = {
    {"Badger Meat", "Bear Meat", "Deer Meat", "Fox Meat", "Rabbit Meat",
     "Squirrel Meat", "Barberry", "Bindweed", "Buckthorn", "Chokecherry",
     "Common Burdock", "Dandelion", "Mountain Deathcamas", "Oregon Grape",
     "Purslane", "Stinging Nettle", "Wild Onion", "Wild Strawberry"},
    {0},
    {20, 100, 40, 20, 10, 10, 2, -2, -5, 3, 5, 5, -100, 2, 10, 2, 2},

    {"Bearskin Cloak", "Fox Skin Gloves", "Leather Gloves",
     "Rabbit Skin Gloves", "Badger Hat", "Felt Hat", "Fox Hat", "Raccoon Hat",
     "Heavy Cotton Shirt", "Heavy Cotton Trousers", "Heavy Wool Vest",
     "Moccasins", "Bearskin Boots", "Deerskin Boots", "Rabbit Boots"},
    {0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0},
    {0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0},
    {50, 7, 5, 8, 7, 5, 8, 7, 5, 8, 7, 10, 20, 20, 10, 30, 20, 15},
    {"Axe", "Kettle", "Knife"},
    {0},
    {0},
    {false},
    {"Purified Water", "River Water", "Wood"},
    {0},
    {"Badger", "Bear", "Deer", "Fox", "Rabbit", "Squirrel"},
    {0}};

void clearScreen() {
#ifdef _WIN32 
system("cls");
#else
system("clear");
#endif
}

void listStats(struct playerStats *player) {
  clearScreen();
  char *statNames[20] = {"Health",      "Energy", "Hunger",   "Thirst",
                         "Temperature", "Charm",  "Quickness"};
  printf("Health: %d\n", player->health);
  printf("Energy: %d\n", player->energy);
  printf("Hunger: %d\n", player->hunger);
  printf("Thirst: %d\n", player->thirst);
  printf("Temperature: %d\n", player->temperature);
  printf("Charm: %d\n", player->charm);
  printf("Quickness: %d\n", player->quickness);
}

void warnings(struct playerStats *player) {
  clearScreen();
  if (player->temperature < 20) {
    printf("I won't survive this cold much longer.\n");
  } else if (20 <= player->temperature && player->temperature < 50) {
    printf("The cold is becoming unbearable.\n")
  } else if (50 <= player->temperature && player->temperature < 70) {
    printf("The cold is starting to get to me.\n");
  }

  if (player->health < 20) {
    printf("My health is quickly deteriorating.\n");
  } else if (20 <= player->health && player->health < 50) {
    printf("I feel very ill.\n");
  } else if (50 <= player->health && player->health < 70) {
    printf("I feel a little under the weather.\n");
  }

  if (player->hunger < 20) {
    printf("I am starving.\n");
  } else if (20 <= player->hunger && player->hunger < 50) {
    printf("I need to eat something soon.\n");
  } else if (50 <= player->hunger && player->hunger < 70) {
    printf("I am starting to get hungry.\n");
  }

  if (player->thirst < 20) {
    printf("My throat is very dry.\n");
  } else if (20 <= player->thirst && player->thirst < 50) {
    printf("I need to drink something soon.\n");
  } else if (50 <= player->thirst && player->thirst < 70) {
    printf("I am starting to get thirsty.\n");
  }
}

void travelMenu() {
  clearScreen();
  printf("I walked to...\n");
  printf("Press Enter to continue");
  while (getchar() != '\n')
    ;
}

void displayInventory() {
  clearScreen();
  printf("%s:%d\n", inventory.clothing[0][1], inventory.clothingPossessed[0]);
}

// Calculate whether to lower player's stats at end of day 
void advanceDay() {
	printf("Advancing day...\n");
}

void homeMenu() {
  char homeSelection[3];
  int day = 1;
  int climate = 23;
  char conditions[20] = "snowed";
  int leftHome = 0;
  while (!leftHome) {
    clearScreen();
    printf("Day %d\n\n", day);
    printf("It %s today.\n", conditions);
    printf("I decided to...\n\n");
    printf("1. Leave Camp\n2. Examine My Belongings\n3. Reflect\n4. List Stats"
           "(Debug)\n5. Sleep\n");
    fgets(homeSelection, 3, stdin);
    homeSelection[strcspn(homeSelection, "\n")] = '\0';
    if (strcmp(homeSelection, "1") == 0) {
      travelMenu();
      int leftHome = 1;
    } else if (strcmp(homeSelection, "2") == 0) {
      displayInventory();
      printf("Press Enter to continue");
      while (getchar() != '\n')
        ;
    } else if (strcmp(homeSelection, "3") == 0) {
      warnings(&player);
      printf("Press Enter to continue");
      while (getchar() != '\n')
        ;
    } else if (strcmp(homeSelection, "4") == 0) {
      listStats(&player);
      printf("Press Enter to continue");
      while (getchar() != '\n')
        ;
	} else if (strcmp(homeSelection, "5") == 0) {
      advanceDay();
      printf("Press Enter to continue");
      while (getchar() != '\n')
        ;
    } else {
      printf("Invalid Input");
    }
  }
}

void preface() {
  printf("Someday you'll find yourself compelled to disrupt a patch of dirt, "
         "thinking you'll dig up a better tomorrow.\n\n");
  sleep(2);
  printf("You'll dig until your hands are masses of blisters and your as "
         "stiff as a board. Another hour and I'll certainly be there, you'll "
         "think to yourself.\n\n");
  sleep(2);
  printf("The sun might try to boil your skin, but you'll persist.\n\n");
  sleep(2);
  printf("The truth is, some holes don't want to be dug.\n\n");
  sleep(2);
  printf("Eventually there comes a time where you just have to put down the "
         "shovel and be on your way, or that hole will be your grave.\n\n");
  sleep(2);
  clearScreen();
}

void gameplay() { homeMenu(); }

void mainMenu() {
  char mainSelection[3];
  char prefaceSelection[3];
  printf("1. New Journal\n2. Continue Journal\n3. Go Home\n");
  fgets(mainSelection, 3, stdin);
  mainSelection[strcspn(mainSelection, "\n")] = '\0';
  if (strcmp(mainSelection, "1") == 0) {
    clearScreen();
    printf("1. Read Preface\n2. Skip Preface\n");
    fgets(prefaceSelection, 3, stdin);
    prefaceSelection[strcspn(prefaceSelection, "\n")] = '\0';
    if (strcmp(prefaceSelection, "1") == 0) {
      preface();
    }
    gameplay();
  } else if (strcmp(mainSelection, "2") == 0) {
    printf("Saving not yet implemented\n");
  } else if (strcmp(mainSelection, "3") == 0) {
    return;
  } else {
    printf("Invalid Choice\n");
  }
}

int main() {
  mainMenu();
  return EXIT_SUCCESS;
}