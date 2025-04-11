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
#define INVENTORY_ARRAY_OFFSET 1

struct item {
  char name[50];
  int amount;
  int equipped;
  int effects;
  char type[50];
};

struct player_inventory {
  struct item items[NUM_OF_ITEMS];
};

struct inventory_info {
  char items_possessed[NUM_OF_ITEMS][ITEM_NAME_SIZE];
  int item_count;
  int num_of_items_possessed;
};

struct player_stats {
  int health;
  int energy;
  int hunger;
  int thirst;
  int warmth;
  int charm;
};

struct world_stats {
  int day;
  int current_temperature;
  char current_weather[255];
};

extern struct inventory_info current_inventory_info;
extern struct player_inventory inventory;
extern struct player_stats player;
extern struct world_stats world;

// Input Functions
int input_is_numeric(const char *input);
int prompt(char *prompt_msg);

// Inventory Functions
void discard_item(int selected_item_index);
int get_item_index(char *selected_item_name);
void reset_inventory_info();
void use_item(int selected_item_index);
void warnings(void);

// Menu Functions
void main_menu(void);
void travel_menu(void);

// Logic Functions
void advance_day(void);
int generate_temperature(void);
char *generate_inventory_prompt(char *generated_inventory_prompt);
char *generate_weather(void);
void travel_checks(void);

// Utility Functions
void clear_screen(void);
void clear_stdin(void);
void invalid_input_msg(void);

#endif