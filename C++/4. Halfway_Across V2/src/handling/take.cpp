#include "../../include/audio.h"
#include "../../include/handling.h"
#include "../../include/player.h"

void handling::take(MainWindow *mainWindow, QString target,
                    Location *location) {
  int itemIndex = location->searchInventory(target.toUpper());
  if (itemIndex != -1) {
    sfxPlayer.play("qrc:/audio/sfx/take.mp3", 0);
    mainWindow->setDescription(
        QString("Took %1.").arg(location->getItemName(itemIndex).toLower()));
    int playerItemIndex = player.searchInventory(target.toUpper());
    if (playerItemIndex != -1) {
      player.setItemQuantity(playerItemIndex,
                             player.getItemQuantity(playerItemIndex) + 1);
    } else {
      player.addItem(location->getInventoryItem(itemIndex));
    }
    if (location->getItemQuantity(itemIndex) == 1) {
      location->removeItem(itemIndex);
    } else {
      location->setItemQuantity(itemIndex,
                                location->getItemQuantity(itemIndex) - 1);
    }
  } else {
    mainWindow->setDescription("You don't know what to take.");
  }
}
