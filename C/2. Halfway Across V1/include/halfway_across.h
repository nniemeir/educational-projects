#ifndef HALFWAY_ACROSS_H_
#define HALFWAY_ACROSS_H_
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define NUM_OF_ITEMS 5
#define ITEM_NAME_SIZE 255
#define PROMPT_SIZE 255

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

struct inventoryValues {
char itemsPossessed[NUM_OF_ITEMS][ITEM_NAME_SIZE];
int itemCount;
int numOfItemsPossessed;
long selection;
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
extern struct inventoryValues currentInventoryValues;

void clearScreen();
void clearStdin();
void invalidInputMessage();
int inputIsNumeric(const char *input);
int findItemIndex(char *selectedItemName);
void useItem(int selectedItemIndex);
void discardItem(int selectedItemIndex);
void gameplay();
void warnings();
void mainMenu();
int generateTemperature();
char *generateWeather();
void advanceDay();

#endif