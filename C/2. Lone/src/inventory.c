#include "lone.h"

struct playerInventory inventory = {{
    {"Badger Meat", 0, 0, 20, "food"},
    {"Bear Meat", 0, 0, 100, "food"},
    {"Deer Meat", 0, 0, 40, "food"},
    {"Fox Meat", 0, 0, 20, "food"},
    {"Rabbit Meat", 0, 0, 10, "food"},
    {"Squirrel Meat", 0, 0, 10, "food"},
    {"Barberry", 0, 0, 2, "food"},
    {"Bindweed", 0, 0, -2, "food"},
    {"Buckthorn", 0, 0, -5, "food"},
    {"Chokecherry", 0, 0, 3, "food"},
    {"Common Burdock", 0, 0, 5, "food"},
    {"Dandelion", 0, 0, 5, "food"},
    {"Mountain Deathcamas", 0, 0, -100, "food"},
    {"Oregon Grape", 0, 0, 2, "food"},
    {"Purslane", 0, 0, 2, "food"},
    {"Stinging Nettle", 0, 0, 10, "food"},
    {"Wild Onion", 0, 0, 2, "food"},
    {"Wild Strawberry", 0, 0, 2, "food"},
    {"Bearskin Cloak", 0, 0, 50, "clothing"},
    {"Fox Skin Gloves", 0, 0, 7, "clothing"},
    {"Leather Gloves", 1, 1, 5, "clothing"},
    {"Rabbit Skin Gloves", 0, 0, 8, "clothing"},
    {"Badger Hat", 0, 0, 7, "clothing"},
    {"Felt Hat", 1, 1, 5, "clothing"},
    {"Fox Hat", 0, 0, 8, "clothing"},
    {"Raccoon Hat", 0, 0, 7, "clothing"},
    {"Heavy Cotton Shirt", 1, 1, 10, "clothing"},
    {"Heavy Cotton Trousers", 1, 1, 20, "clothing"},
    {"Heavy Wool Vest", 0, 0, 20, "clothing"},
    {"Moccasins", 1, 1, 10, "clothing"},
    {"Bearskin Boots", 0, 0, 30, "clothing"},
    {"Dearskin Boots", 0, 0, 20, "clothing"},
    {"Rabbit Boots", 0, 0, 15, "clothing"},
    {"Axe", 0, 0, 0, "tool"},
    {"Kettle", 0, 0, 0, "tool"},
    {"Knife", 0, 0, 0, "tool"},
    {"Purified Water", 0, 0, 50, "liquid"},
    {"River Water", 0, 0, 25, "liquid"},
    {"Wood", 0, 0, 0, "resource"},
    {"Badger pelt", 0, 0, 0, "resource"},
    {"Bear pelt", 0, 0, 0, "resource"},
    {"Deer pelt", 0, 0, 0, "resource"},
    {"Fox pelt", 0, 0, 0, "resource"},
    {"Rabbit pelt", 0, 0, 0, "resource"},
    {"Squirrel pelt", 0, 0, 0, "resource"},
}};

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
  fprintf(stderr, "Error occurred when finding item index from name.\n");
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
    player.thirst = player.thirst + inventory.items[selectedItemIndex].effects;
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