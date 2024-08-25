#include "../../include/handling.h"
#include "../../include/player.h"
#include "../../include/world.h"

void handling::read(MainWindow *mainWindow, QString target, QString location) {
    if (location == "camp") {
        readCamp(mainWindow, target);
    } else if (location == "campPath") {
        readCampPath(mainWindow, target);
    } else if (location == "cave") {
        readCave(mainWindow, target);
    } else if (location == "caveEntrance") {
        readCaveEntrance(mainWindow, target);
    } else if (location == "lake") {
        readLake(mainWindow, target);
    } else if (location == "valley") {
        readValley(mainWindow, target);
    }
    else {
        mainWindow->setDescription("You can't do that here.");
    }
}

void handling::readCamp(MainWindow *mainWindow, QString target) {

}

void handling::readCampPath(MainWindow *mainWindow, QString target) {
}

void handling::readCave(MainWindow *mainWindow, QString target) {
}

void handling::readCaveEntrance(MainWindow *mainWindow, QString target) {
}

void handling::readLake(MainWindow *mainWindow, QString target) {
}

void handling::readValley(MainWindow *mainWindow, QString target) {
}
