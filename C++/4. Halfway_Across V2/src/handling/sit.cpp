#include "../../include/handling.h"
#include "../../include/player.h"

void handling::sit(MainWindow *mainWindow, QString target, Location *location) {
  if (location->getName() == "camp") {
    sitCamp(mainWindow, target);
  } else {
    mainWindow->setDescription("You can't do that here.");
  }
}

void handling::sitCamp(MainWindow *mainWindow, QString target) {
  if (target == "ON LOG") {
    mainWindow->setDescription("You feel a bit warmer sitting by the fire.");
    player.setStanding(0);
  }
  else if (target == "ON FIRE") {
      mainWindow->setDescription("You burned yourself.");
      player.setHealth(player.getHealth() - 20);
  } else {
    mainWindow->setDescription("You can't sit there.");
  }
}
