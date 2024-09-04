#include "../../include/handling.h"
#include "../../include/player.h"
#include "../../include/world.h"

void handling::look(MainWindow *mainWindow, QString target,
                    Location *location) {
  if (location->getName() == "camp") {
    lookCamp(mainWindow, target);
  } else if (location->getName() == "campPath") {
    lookCampPath(mainWindow, target, location);
  } else if (location->getName() == "cave") {
    lookCave(mainWindow, target);
  } else if (location->getName() == "caveEntrance") {
    lookCaveEntrance(mainWindow, target);
  } else if (location->getName() == "lake") {
    lookLake(mainWindow, target);
  } else if (location->getName() == "valley") {
    lookValley(mainWindow, target);
  } else {
    mainWindow->setDescription("You can't do that here.");
  }
}

void handling::lookCamp(MainWindow *mainWindow, QString target) {
  QMap<QString, QString> descriptions = {
      {"AROUND", camp.getDescription()},
      {"AT FIRE", "The fire burns low."},
      {"AT BED", "The bed seems to be quite worn."},
      {"AT CHEST", "The rusty chest contains your belongings."},
      {"AT GROUND", camp.locInventory()},
      {"IN BAG", player.bagInventory()},
      {"AT SELF", player.clothesInventory()},
      {"OUTSIDE",
       QString("It is %1 outside.").arg(world.getCurrentWeather().toLower())}};

  if (descriptions.contains(target)) {
    mainWindow->setDescription(descriptions.value(target));
  } else {
      mainWindow->setDescription(QString("You can't look %1 here.\n").arg(target.toLower()));
  }
}

void handling::lookCampPath(MainWindow *mainWindow, QString target,
                            Location *location) {
  QMap<QString, QString> descriptions = {
      {"AROUND", location->getDescription()},
      {"AT GROUND", location->locInventory()},
      {"AT PATH", "The other branches of the path go off into the distance.\n"},
      {"IN BAG", player.bagInventory()},
      {"AT SELF", player.clothesInventory()}};

  if (descriptions.contains(target)) {
    mainWindow->setDescription(descriptions.value(target));
  } else {
      mainWindow->setDescription(QString("You can't look %1 here.\n").arg(target.toLower()));
  }
}

void handling::lookCave(MainWindow *mainWindow, QString target) {
  QMap<QString, QString> descriptions = {
      {"AROUND", valley.getDescription()},
      {"AT GROUND", cave.locInventory()},

      {"IN BAG", player.bagInventory()},
      {"AT SELF", player.clothesInventory()}};
  if (descriptions.contains(target)) {
    mainWindow->setDescription(descriptions.value(target));
  } else {
    mainWindow->setDescription(QString("You can't look %1 here.\n").arg(target.toLower()));
  }
}

void handling::lookCaveEntrance(MainWindow *mainWindow, QString target) {
  QMap<QString, QString> descriptions = {
      {"AROUND", caveEntrance.getDescription()},
      {"AT GROUND", caveEntrance.locInventory()},

      {"IN BAG", player.bagInventory()},
      {"AT SELF", player.clothesInventory()}};
  if (descriptions.contains(target)) {
    mainWindow->setDescription(descriptions.value(target));
  } else {
      mainWindow->setDescription(QString("You can't look %1 here.\n").arg(target.toLower()));
  }
}

void handling::lookLake(MainWindow *mainWindow, QString target) {
  QMap<QString, QString> descriptions = {
      {"AROUND", lake.getDescription()},
      {"AT GROUND", lake.locInventory()},
      {"AT LAKE", "The lake has frozen over.\n"},
      {"AT MOUNTAIN", "Who knows what that mountain holds?\n"},

      {"IN BAG", player.bagInventory()},
      {"AT SELF", player.clothesInventory()}};

  if (descriptions.contains(target)) {
    mainWindow->setDescription(descriptions.value(target));
  } else {
      mainWindow->setDescription(QString("You can't look %1 here.\n").arg(target.toLower()));
  }
}

void handling::lookValley(MainWindow *mainWindow, QString target) {
  QMap<QString, QString> descriptions = {
      {"AROUND", valley.getDescription()},
      {"AT GROUND", valley.locInventory()},
      {"AT TREES", "The trees are covered in snow."},

      {"IN BAG", player.bagInventory()},
      {"AT SELF", player.clothesInventory()}};
  if (descriptions.contains(target)) {
    mainWindow->setDescription(descriptions.value(target));
  } else {
      mainWindow->setDescription(QString("You can't look %1 here.\n").arg(target.toLower()));
  }
}
