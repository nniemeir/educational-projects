#include "../../include/handling.h"
#include "../../include/player.h"
#include "../../include/world.h"

void handling::use(MainWindow *mainWindow, QString target, Location* location) {
    if (location->getName() == "lake") {
    useLake(mainWindow, target);
  } else {
    mainWindow->setDescription("You can't do that here.");
  }
}

void handling::useLake(MainWindow *mainWindow, QString target) {
  if (target == "FISHING ROD") {
    if (player.searchInventory("FISHING ROD") != -1) {
        if (world.getChiseledIce()) {
        mainWindow->setDescription(
          "You drop your line into the hole in the ice.\n");
    } else {
      mainWindow->setDescription(
          "You do not know where to use the fishing rod.\n");
    }
    } else {
        mainWindow->setDescription(
            "You don't have a fishing rod\n");
    }
  }
}
