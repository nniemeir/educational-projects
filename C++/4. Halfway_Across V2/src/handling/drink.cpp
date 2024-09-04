#include "../../include/handling.h"
#include "../../include/player.h"

void handling::drink(MainWindow *mainWindow, QString target,
                     Location *location) {
  if (location->getName() == "lake") {
    drinkLake(mainWindow, target);
  } else {
    mainWindow->setDescription("You can't do that here.");
  }
}

void handling::drinkLake(MainWindow *mainWindow, QString target) {
  if (target == "FROM LAKE") {
    mainWindow->setDescription(
        "You feel more refreshed drinking the cold water");
    player.setThirst(player.getThirst() + 20);
  }
}
