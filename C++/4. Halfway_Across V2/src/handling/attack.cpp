#include "../../include/handling.h"
#include "../../include/player.h"
#include "../../include/world.h"

void handling::attack(MainWindow *mainWindow, QString target, QString location) {
     if (location == "campPath") {
        attackCampPath(mainWindow, target);
    } else if (location == "cave") {
        attackCave(mainWindow, target);
    } else if (location == "caveEntrance") {
        attackCaveEntrance(mainWindow, target);
    } else if (location == "lake") {
        attackLake(mainWindow, target);
    } else if (location == "valley") {
        attackValley(mainWindow, target);
    }
    else {
        mainWindow->setDescription("You can't do that here.");
    }
}

void handling::attackCampPath(MainWindow *mainWindow, QString target) {
}

void handling::attackCave(MainWindow *mainWindow, QString target) {
}

void handling::attackCaveEntrance(MainWindow *mainWindow, QString target) {
}

void handling::attackLake(MainWindow *mainWindow, QString target) {
}

void handling::attackValley(MainWindow *mainWindow, QString target) {
}
