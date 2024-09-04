#include "../../include/audio.h"
#include "../../include/handling.h"
#include "../../include/player.h"
#include "../../include/world.h"

void handling::wait(MainWindow *mainWindow, Location *location) {
  if (location->getName() == "lake") {
    waitLake(mainWindow);
  } else {
    mainWindow->setDescription("You can't do that here.");
  }
}

void handling::waitLake(MainWindow *mainWindow) {
  if (world.getLineSet() == 1) {
    QString generatedFish = world.generateFish();
    int itemIndex = world.searchFish(generatedFish);
    if (itemIndex != -1) {
      sfxPlayer.play("qrc:/audio/sfx/fishReel.mp3", 0);
      mainWindow->setDescription(
          QString("You caught a %1!\n")
              .arg(world.getFishName(itemIndex).toLower()));
      int playerItemIndex = player.searchInventory(generatedFish);
      if (playerItemIndex != -1) {
        player.setItemQuantity(playerItemIndex,
                               player.getItemQuantity(playerItemIndex) + 1);
      } else {
        player.addItem(world.getFish(itemIndex));
      }
    }
    world.setLineSet(0);
  }
}
