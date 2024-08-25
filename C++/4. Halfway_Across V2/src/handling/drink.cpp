#include "../../include/handling.h"
#include "../../include/player.h"
#include "../../include/world.h"

void handling::drink(MainWindow *mainWindow, QString target, QString location) {
    if (location == "camp") {
        drinkCamp(mainWindow, target);
    } else if (location == "campPath") {
        drinkCampPath(mainWindow, target);
    } else if (location == "cave") {
        drinkCave(mainWindow, target);
    } else if (location == "caveEntrance") {
        drinkCaveEntrance(mainWindow, target);
    } else if (location == "lake") {
        drinkLake(mainWindow, target);
    } else if (location == "valley") {
        drinkValley(mainWindow, target);
    }
    else {
        mainWindow->setDescription("You can't do that here.");
    }
}


void handling::drinkCamp(MainWindow *mainWindow, QString target) {
}

void handling::drinkCampPath(MainWindow *mainWindow, QString target) {
}

void handling::drinkCave(MainWindow *mainWindow, QString target) {
}

void handling::drinkCaveEntrance(MainWindow *mainWindow, QString target) {
}

void handling::drinkLake(MainWindow *mainWindow, QString target) {
    if (target == "FROM LAKE") {
        mainWindow->setDescription("You feel more refreshed drinking the cold water");
        player.thirst = player.thirst + 20;
        player.thirst = player.constrainStat(player.thirst);
    }
}

void handling::drinkValley(MainWindow *mainWindow, QString target) {
}



