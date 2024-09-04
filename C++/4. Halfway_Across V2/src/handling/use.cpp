#include "../../include/audio.h"
#include "../../include/handling.h"
#include "../../include/player.h"
#include "../../include/world.h"

void handling::use(MainWindow *mainWindow, QString target, Location *location) {
  if (location->getName() == "lake") {
    useLake(mainWindow, target);
  } else {
    mainWindow->setDescription("You can't do that here.");
  }
}

void handling::useLake(MainWindow *mainWindow, QString target) {
  if (target == "CHISEL") {
    if (player.searchInventory("CHISEL") != -1) {
      sfxPlayer.play("qrc:/audio/sfx/chiselLake.mp3", 0);
      mainWindow->setDescription("You chisel a hole in the ice.\n");
      world.setChiseledIce(1);
    } else {
      mainWindow->setDescription("You don't have a chisel.");
    }
  } else if (target == "FISHING ROD") {
    if (player.searchInventory("FISHING ROD") != -1) {
      if (world.getChiseledIce()) {
        sfxPlayer.play("qrc:/audio/sfx/fishSet.mp3", 0);
        world.setLineSet(1);
        mainWindow->setDescription(
            "You drop your line into the hole in the ice.\n");
      } else {
        mainWindow->setDescription(
            "You do not know where to use the fishing rod.\n");
      }
    } else {
      mainWindow->setDescription("You don't have a fishing rod\n");
    }
  }
}
