#include "../include/halfway_across.h"

struct player_inventory inventory = {{
    {"Leather Gloves", 1, 1, 5, "clothing"},
    {"Felt Hat", 1, 1, 5, "clothing"},
    {"Heavy Cotton Shirt", 1, 1, 10, "clothing"},
    {"Heavy Cotton Trousers", 1, 1, 20, "clothing"},
    {"Moccasins", 1, 1, 10, "clothing"},
}};

// The name of the item selected by the player is used to find which item struct
// instance it corresponds to within the inventory struct
int get_item_index(char *selected_item_name) {
  int selected_item_index;
  int items_increment;
  for (items_increment = 0; items_increment < NUM_OF_ITEMS; items_increment++) {
    if (strcmp(inventory.items[items_increment].name, selected_item_name) ==
        0) {
      selected_item_index = items_increment;
      return selected_item_index;
    }
  }
  fprintf(stderr, "Error occurred when finding item index from name.\n");
  exit(EXIT_FAILURE);
}

// The stat that is modified depends on the type of the item.
// Using a consumable subtracts one of it from your inventory
// However, using a clothing item equips/unequips it
void use_item(int selected_item_index) {
  clear_screen();
  if (strcmp(inventory.items[selected_item_index].type, "clothing") != 0) {
    fprintf(stderr, "Invalid inventory entry.\n");
    return;
  }
  if (!inventory.items[selected_item_index].equipped) {
    printf("I put on my %s...\n", inventory.items[selected_item_index].name);
    inventory.items[selected_item_index].equipped = 1;
    player.warmth =
        player.warmth + inventory.items[selected_item_index].effects;
  } else {
    printf("I took off my %s...\n", inventory.items[selected_item_index].name);
    inventory.items[selected_item_index].equipped = 0;
    player.warmth =
        player.warmth - inventory.items[selected_item_index].effects;
  }
  clear_stdin();
  return;
}

// Removes x of an item from the inventory if x is <= the amount currently
// possessed
void discard_item(int selected_item_index) {
  int is_valid_amount = 0;
  while (!is_valid_amount) {
    clear_screen();
    char discard_prompt[PROMPT_SIZE];
    snprintf(discard_prompt, PROMPT_SIZE, "I had %d, so I discarded...\n> ",
             inventory.items[selected_item_index].amount);
    long discard_amount = prompt(discard_prompt);
    if (discard_amount < 0) {
      invalid_input_msg();
      continue;
    }
    if (discard_amount >= 0 &&
        discard_amount <= inventory.items[selected_item_index].amount) {
      is_valid_amount = 1;
      if (inventory.items[selected_item_index].equipped == 1 &&
          discard_amount > 0) {
        use_item(selected_item_index);
      }
      inventory.items[selected_item_index].amount =
          inventory.items[selected_item_index].amount - discard_amount;
      return;
    }
    clear_screen();
    printf("I didn't have enough to discard that many...\n");
    clear_stdin();
    clear_screen();
  }
}

// Enumerates items that the player possesses one or more of
char *generate_inventory_prompt(char *prompt_buffer) {
  snprintf(prompt_buffer, PROMPT_SIZE,
           "I had a variety of items, I inspected my...\n\n");
  char item_number[100];
  for (int i = 0; i < NUM_OF_ITEMS; i++) {
    if (inventory.items[i].amount != 0) {
      snprintf(current_inventory_info
                   .items_possessed[current_inventory_info.item_count],
               sizeof(current_inventory_info
                          .items_possessed[current_inventory_info.item_count]),
               "%s", inventory.items[i].name);

      snprintf(item_number, sizeof(item_number), "%d",
               current_inventory_info.item_count + INVENTORY_ARRAY_OFFSET);
      strncat(prompt_buffer, item_number, strlen(item_number));
      strncat(prompt_buffer, ". ", 3);
      strncat(prompt_buffer, inventory.items[i].name, PROMPT_SIZE);
      strncat(prompt_buffer, "\n", 2);
      current_inventory_info.item_count++;
    }
  }
  snprintf(item_number, sizeof(item_number), "%d",
           current_inventory_info.item_count + INVENTORY_ARRAY_OFFSET);
  strcat(prompt_buffer, item_number);
  strcat(prompt_buffer, ". Exit\n\n> ");
  return prompt_buffer;
}

// The value of each variable in the current_inventory_info struct is altered in
// inventory_menu and generate_inventory_prompt, so they must all be reset
// before running the functions again.
void reset_inventory_info(void) {
  for (int items_possessed_increment = 0;
       items_possessed_increment < NUM_OF_ITEMS; items_possessed_increment++) {
    memset(current_inventory_info.items_possessed[items_possessed_increment],
           '\0', ITEM_NAME_SIZE);
  }
  current_inventory_info.item_count = 0;
  current_inventory_info.num_of_items_possessed = 0;
}