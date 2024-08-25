#include "../../include/handling.h"
#include "../../include/player.h"
#include "../../include/world.h"

void handling::open(MainWindow *mainWindow, QString target, QString location) {
    if (location == "camp") {
        openCamp(mainWindow, target);
    } else if (location == "campPath") {
        openCampPath(mainWindow, target);
    } else if (location == "cave") {
        openCave(mainWindow, target);
    } else if (location == "caveEntrance") {
        openCaveEntrance(mainWindow, target);
    } else if (location == "lake") {
        openLake(mainWindow, target);
    } else if (location == "valley") {
        openValley(mainWindow, target);
    }
    else {
        mainWindow->setDescription("You can't do that here.");
    }
}

void handling::openCamp(MainWindow *mainWindow, QString target) {

}

void handling::openCampPath(MainWindow *mainWindow, QString target) {
}

void handling::openCave(MainWindow *mainWindow, QString target) {
}

void handling::openCaveEntrance(MainWindow *mainWindow, QString target) {
}

void handling::openLake(MainWindow *mainWindow, QString target) {
}

void handling::openValley(MainWindow *mainWindow, QString target) {
}
