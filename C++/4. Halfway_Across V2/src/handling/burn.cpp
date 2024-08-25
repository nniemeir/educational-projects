#include "../../include/handling.h"
#include "../../include/player.h"
#include "../../include/world.h"

void handling::burn(MainWindow *mainWindow, QString target, QString location) {
    if (location == "camp") {
        burnCamp(mainWindow, target);
    } else if (location == "campPath") {
        burnCampPath(mainWindow, target);
    } else if (location == "cave") {
        burnCave(mainWindow, target);
    } else if (location == "caveEntrance") {
        burnCaveEntrance(mainWindow, target);
    } else if (location == "lake") {
        burnLake(mainWindow, target);
    } else if (location == "valley") {
        burnValley(mainWindow, target);
    }
    else {
        mainWindow->setDescription("You can't do that here.");
    }
}

void handling::burnCamp(MainWindow *mainWindow, QString target) {

}

void handling::burnCampPath(MainWindow *mainWindow, QString target) {
}

void handling::burnCave(MainWindow *mainWindow, QString target) {
}

void handling::burnCaveEntrance(MainWindow *mainWindow, QString target) {
}

void handling::burnLake(MainWindow *mainWindow, QString target) {
}

void handling::burnValley(MainWindow *mainWindow, QString target) {
}
