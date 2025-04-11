#include "../include/halfway_across.h"

typedef enum { MAIN_NEW_JOURNAL = 1, MAIN_GO_HOME } main_menu_options;

typedef enum {
  CAMP_LEAVE = 1,
  CAMP_SHOW_INVENTORY,
  CAMP_REFLECT,
  CAMP_ADVANCE_DAY,
  CAMP_GIVE_UP
} camp_menu_options;

typedef enum {
  TRAVEL_LAKE = 1,
  TRAVEL_VALLEY,
  TRAVEL_NOWHERE
} travel_menu_options;

typedef enum { VALLEY_REFLECT = 1, VALLEY_LEAVE } valley_menu_options;

typedef enum { LAKE_REFLECT = 1, LAKE_LEAVE } lake_menu_options;

typedef enum { ITEM_USE = 1, ITEM_DISCARD, ITEM_EXIT } item_menu_options;

struct inventory_info current_inventory_info = {{"nothing"}, 0, 0};

void clear_screen(void) { printf("\033c"); }

void clear_stdin(void) {
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;
}

static void daily_msg(void) {
  printf("San Juan Mountains, 1893\n");
  printf("Day %d\n\n", world.day);
  printf("It was %d degrees and %s today.\n", world.current_temperature,
         world.current_weather);
}

static void death_msg(void) {
  clear_screen();
  printf("This is my last entry, my health is failing me. Whoever finds "
         "this journal is welcome to whatever is left in my camp...");
  clear_stdin();
  clear_screen();
}

void invalid_input_msg(void) {
  clear_screen();
  printf("My memory is failing me at the moment...\n");
  clear_stdin();
  clear_screen();
}

static void item_menu(char *selected_item_name) {
  int selected_item_index = get_item_index(selected_item_name);
  int valid_item_menu_selection = 0;
  while (!valid_item_menu_selection) {
    clear_screen();
    printf("%s\nQuantity: %d\nEquipped: %d\n\n", selected_item_name,
           inventory.items[selected_item_index].amount,
           inventory.items[selected_item_index].equipped);
    int selection = prompt("1. Use / Equip\n2. Discard\n3. Exit\n\n> ");
    switch (selection) {
    case ITEM_USE:
      use_item(selected_item_index);
      valid_item_menu_selection = 1;
      break;
    case ITEM_DISCARD:
      discard_item(selected_item_index);
      valid_item_menu_selection = 1;
      break;
    case ITEM_EXIT:
      valid_item_menu_selection = 1;
      clear_screen();
      printf("I chose to leave it be...\n");
      break;
    default:
      invalid_input_msg();
    }
  }
}

static void inventory_menu(void) {
  int valid_inventory_selection = 0;
  while (!valid_inventory_selection) {
    reset_inventory_info();
    clear_screen();
    char prompt_buffer[PROMPT_SIZE];
    char *inventory_prompt = generate_inventory_prompt(prompt_buffer);
    int selection = prompt(inventory_prompt);
    int exit_option =
        current_inventory_info.item_count + INVENTORY_ARRAY_OFFSET;
    if (selection > 0 && selection <= current_inventory_info.item_count) {
      int offset_selection = selection - INVENTORY_ARRAY_OFFSET;
      item_menu(current_inventory_info.items_possessed[offset_selection]);
      continue;
    }
    if (selection == exit_option) {
      valid_inventory_selection = 1;
      clear_screen();
      printf("I switched my focus to another matter...\n");
      return;
    }
    invalid_input_msg();
  }
}

static void lake_menu(void) {
  int left_lake = 0;
  while (!left_lake) {
    clear_screen();
    int selection = prompt("After reaching the lake, I decided to...\n\n1. "
                           "Reflect\n2. Return to Camp\n\n> ");
    switch (selection) {
    case LAKE_REFLECT:
      warnings();
      break;
    case LAKE_LEAVE:
      left_lake = 1;
      break;
    default:
      invalid_input_msg();
    }
  }
}

static void valley_menu(void) {
  int left_valley = 0;
  while (!left_valley) {
    clear_screen();
    int selection = prompt("After reaching the valley, I decided to...\n\n1. "
                           "Reflect\n2. Return to Camp\n\n> ");
    switch (selection) {
    case VALLEY_REFLECT:
      warnings();
      break;
    case VALLEY_LEAVE:
      left_valley = 1;
      break;
    default:
      invalid_input_msg();
    }
  }
}

void travel_menu(void) {
  int valid_travel_menu_selection = 0;
  while (!valid_travel_menu_selection) {
    clear_screen();
    int selection = prompt("I decided to walk to...\n\n1. The Lake\n2. The "
                           "Valley\n3. Nowhere\n\n> ");
    switch (selection) {
    case TRAVEL_LAKE:
      valid_travel_menu_selection = 1;
      player.energy = 0;
      lake_menu();
      break;
    case TRAVEL_VALLEY:
      valid_travel_menu_selection = 1;
      player.energy = 0;
      valley_menu();
      break;
    case TRAVEL_NOWHERE:
      valid_travel_menu_selection = 1;
      break;
    default:
      invalid_input_msg();
    }
  }
}

static void camp_menu(void) {
  int gave_up = 0;
  while (!gave_up) {
    clear_screen();
    if (player.health <= 0) {
      death_msg();
      return;
    }
    daily_msg();
    int selection = prompt("I decided to...\n\n1. Leave Camp\n2. Examine My "
                           "Belongings\n3. Reflect\n4. Sleep\n5. "
                           "Give Up\n\n> ");
    switch (selection) {
    case CAMP_LEAVE:
      travel_checks();
      break;
    case CAMP_SHOW_INVENTORY:
      inventory_menu();
      clear_stdin();
      break;
    case CAMP_REFLECT:
      warnings();
      break;
    case CAMP_ADVANCE_DAY:
      advance_day();
      break;
    case CAMP_GIVE_UP:
      gave_up = 1;
      clear_screen();
      printf("The journal ends here...\n");
      clear_stdin();
      break;
    default:
      invalid_input_msg();
    }
  }
}

void main_menu(void) {
  int valid_main_menu_selection = 0;
  while (!valid_main_menu_selection) {
    clear_screen();
    int selection =
        prompt("Halfway Across\n\n1. Begin Journal\n2. Go Home\n\n> ");
    switch (selection) {
    case MAIN_NEW_JOURNAL:
      valid_main_menu_selection = 1;
      camp_menu();
      break;
    case MAIN_GO_HOME:
      valid_main_menu_selection = 1;
      break;
    default:
      invalid_input_msg();
    }
  }
}