#include "../../include/audio.h"
#include "../../include/handling.h"
#include "../../include/player.h"
#include "../../include/world.h"

void handling::move(MainWindow *mainWindow, QString target,
                    Location *location) {
    if (player.getStanding() == 1) {
    if (world.validDirection(target))
    {
    if (location->getName() == "camp") {
      moveCamp(mainWindow, target);
    } else if (location->getName() == "campPath") {
      moveCampPath(mainWindow, target);
    } else if (location->getName() == "caveEntrance") {
      moveCaveEntrance(mainWindow, target);
    } else if (location->getName() == "cave") {
      moveCave(mainWindow, target);
    } else if (location->getName() == "lake") {
      moveLake(mainWindow, target);
    } else if (location->getName() == "valley") {
      moveValley(mainWindow, target);
    } else {
      mainWindow->setDescription("You can't do that here.");
    }
    }
    else {
        mainWindow->setDescription(QString("%1 is not a direction you are aware of.").arg(target.toLower()));
    }
  }
    else {
    mainWindow->setDescription("You have to stand up first.");
  }
}

void handling::moveCamp(MainWindow *mainWindow, QString target) {
  if (target == "NORTH") {
    int canTravel = world.travelChecks();
    switch (canTravel) {
    case TRAVEL_BLIZZARD:
      mainWindow->setDescription(
          "You can't risk traveling in such a blizzard.\n");
      break;
    case TRAVEL_TIRED:
      mainWindow->setDescription(
          "You do not have the energy to travel anywhere else today.\n");
      break;
    case TRAVEL_YES:
      sfxPlayer.play("qrc:/audio/sfx/moveSnow.mp3", 0);
      mainWindow->setLocation(camp.getMusicPath(), camp.getAmbiencePath(),
                              &campPath);
      break;
    }
  } else {
    mainWindow->setDescription(
        QString("You can't move %1 here.\n").arg(target.toLower()));
  }
}

void handling::moveCampPath(MainWindow *mainWindow, QString target) {
    if (target == "WEST") {
    sfxPlayer.play("qrc:/audio/sfx/moveSnow.mp3", 0);
    mainWindow->setLocation(campPath.getMusicPath(), campPath.getAmbiencePath(),
                            &lake);
    player.setEnergy(0);
  } else if (target == "EAST") {
    sfxPlayer.play("qrc:/audio/sfx/moveSnow.mp3", 0);
    mainWindow->setLocation(campPath.getMusicPath(), campPath.getAmbiencePath(),
                            &caveEntrance);
    player.setEnergy(0);
  } else if (target == "NORTH") {
    sfxPlayer.play("qrc:/audio/sfx/moveSnow.mp3", 0);
    mainWindow->setLocation(campPath.getMusicPath(), campPath.getAmbiencePath(),
                            &valley);
    player.setEnergy(0);
  } else if (target == "SOUTH") {
    sfxPlayer.play("qrc:/audio/sfx/moveStone.mp3", 0);
    mainWindow->setLocation(campPath.getMusicPath(), campPath.getAmbiencePath(),
                            &camp);
  } else {
    mainWindow->setDescription(
        QString("You can't move %1 here.\n").arg(target.toLower()));
  }
}

void handling::moveCave(MainWindow *mainWindow, QString target) {
  if (target == "WEST") {
    sfxPlayer.play("qrc:/audio/sfx/moveSnow.mp3", 0);
    mainWindow->setLocation(cave.getMusicPath(), cave.getAmbiencePath(),
                            &caveEntrance);
  } else {
    mainWindow->setDescription(
        QString("You can't move %1 here.\n").arg(target.toLower()));
  }
}

void handling::moveCaveEntrance(MainWindow *mainWindow, QString target) {
  if (target == "WEST") {

    sfxPlayer.play("qrc:/audio/sfx/moveSnow.mp3", 0);
    mainWindow->setLocation(caveEntrance.getMusicPath(),
                            caveEntrance.getAmbiencePath(), &campPath);
  } else if (target == "EAST") {
    sfxPlayer.play("qrc:/audio/sfx/moveStone.mp3", 0);

    mainWindow->setLocation(caveEntrance.getMusicPath(),
                            caveEntrance.getAmbiencePath(), &cave);
  } else {
    mainWindow->setDescription(
        QString("You can't move %1 here.\n").arg(target.toLower()));
  }
}

void handling::moveLake(MainWindow *mainWindow, QString target) {
  if (target == "EAST") {
    sfxPlayer.play("qrc:/audio/sfx/moveSnow.mp3", 0);
    mainWindow->setLocation(lake.getMusicPath(), lake.getAmbiencePath(),
                            &campPath);
  } else {
    mainWindow->setDescription(
        QString("You can't move %1 here.\n").arg(target.toLower()));
  }
}

void handling::moveValley(MainWindow *mainWindow, QString target) {
  if (target == "SOUTH") {
    sfxPlayer.play("qrc:/audio/sfx/moveSnow.mp3", 0);
    mainWindow->setLocation(valley.getMusicPath(), valley.getAmbiencePath(),
                            &campPath);
  } else {
    mainWindow->setDescription(
        QString("You can't move %1 here.\n").arg(target.toLower()));
  }
}
