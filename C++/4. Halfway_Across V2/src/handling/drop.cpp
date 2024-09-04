#include "../../include/handling.h"
#include "../../include/locations.h"
#include "../../include/player.h"

void handling::drop(MainWindow *mainWindow, QString target,
                    Location *location) {
  int itemIndex = player.searchInventory(target.toUpper());
  if (itemIndex != -1) {
    mainWindow->setDescription(
        QString("Dropped %1.").arg(player.getItemName(itemIndex).toLower()));
    int locationItemIndex = location->searchInventory(target.toUpper());
    if (locationItemIndex != -1) {
      location->setItemQuantity(
          locationItemIndex, location->getItemQuantity(locationItemIndex) + 1);
    } else {
      location->addItem(player.getInventoryItem(itemIndex));
    }
    if (player.getItemQuantity(itemIndex) == 1) {
      player.setItemEquipped(itemIndex, 0);
      player.removeItem(itemIndex);
    } else {
      player.setItemQuantity(itemIndex, player.getItemQuantity(itemIndex) - 1);
    }
  } else {
    mainWindow->setDescription("You don't have that.");
  }
}
