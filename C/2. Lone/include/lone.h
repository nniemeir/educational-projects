#ifndef LONE_H_
#define LONE_H_
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define NUM_OF_ITEMS 45
#define ITEM_NAME_SIZE 255
#define PROMPT_SIZE 100

struct item {
  char name[50];
  int amount;
  int equipped;
  int effects;
  char type[50];
};

struct playerInventory {
  struct item items[NUM_OF_ITEMS];
};

struct worldStats {
  int day;
  int currentTemperature;
  char currentWeather[255];
};

struct playerStats {
  int health;
  int energy;
  int hunger;
  int thirst;
  int warmth;
  int charm;
};

extern struct playerInventory inventory;
extern struct playerStats player;
extern struct worldStats world;

void clearScreen();
void clearStdin();
void invalidInputMessage();
void listStats();
int inputIsNumeric(const char *input);
void itemMenu(char *selectedItemName);
int findItemIndex(char *selectedItemName);
void useItem(int selectedItemIndex);
void discardItem(int selectedItemIndex);
void inventoryMenu();
void huntingMenu();
void foragingMenu();
void fishingMenu();
void lakeMenu();
void valleyMenu();
void travelMenu();
void campMenu();
void preface();
void gameplay();
void prefaceMenu();
void saveMenu();
void mainMenu();
void warnings();
int generateTemperature();
char *generateWeather();
void advanceDay();

char *generateAnimal();

#endif