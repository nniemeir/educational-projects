#include "halfway_across.h"

struct playerInventory inventory = {{
    {"Leather Gloves", 1, 1, 5, "clothing"},
    {"Felt Hat", 1, 1, 5, "clothing"},
    {"Heavy Cotton Shirt", 1, 1, 10, "clothing"},
    {"Heavy Cotton Trousers", 1, 1, 20, "clothing"},
    {"Moccasins", 1, 1, 10, "clothing"},
    
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
 if (strcmp(inventory.items[selectedItemIndex].type, "clothing") == 0) {
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
          clearScreen();
          printf("I didn't have enough to discard that many...\n");
          clearStdin();
          clearScreen();
        }
      }
    } else {
      invalidInputMessage();
    }
  }
}