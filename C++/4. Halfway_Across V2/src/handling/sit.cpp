#include "../../include/handling.h"
#include "../../include/player.h"
#include "../../include/world.h"

void handling::sit(MainWindow *mainWindow, QString target, QString location) {
    if (location == "camp") {
        sitCamp(mainWindow, target);
    } else if (location == "campPath") {
        sitCampPath(mainWindow, target);
    } else if (location == "cave") {
        sitCave(mainWindow, target);
    } else if (location == "caveEntrance") {
        sitCaveEntrance(mainWindow, target);
    } else if (location == "lake") {
        sitLake(mainWindow, target);
    } else if (location == "valley") {
        sitValley(mainWindow, target);
    }
    else {
        mainWindow->setDescription("You can't do that here.");
    }
}

void handling::sitCamp(MainWindow *mainWindow, QString target) {
        if (target == "ON LOG") {
            mainWindow->setDescription("You feel a bit warmer sitting by the fire.");
        }
}

void handling::sitCampPath(MainWindow *mainWindow, QString target) {
}

void handling::sitCave(MainWindow *mainWindow, QString target) {
}

void handling::sitCaveEntrance(MainWindow *mainWindow, QString target) {
}

void handling::sitLake(MainWindow *mainWindow, QString target) {
}

void handling::sitValley(MainWindow *mainWindow, QString target) {
}
