#include "../../include/handling.h"
#include "../../include/player.h"
#include "../../include/world.h"

void handling::knock(MainWindow *mainWindow, QString target, QString location) {
    if (location == "camp") {
        knockCamp(mainWindow, target);
    } else if (location == "campPath") {
        knockCampPath(mainWindow, target);
    } else if (location == "cave") {
        knockCave(mainWindow, target);
    } else if (location == "caveEntrance") {
        knockCaveEntrance(mainWindow, target);
    } else if (location == "lake") {
        knockLake(mainWindow, target);
    } else if (location == "valley") {
        knockValley(mainWindow, target);
    }
    else {
        mainWindow->setDescription("You can't do that here.");
    }
}

void handling::knockCamp(MainWindow *mainWindow, QString target) {

}

void handling::knockCampPath(MainWindow *mainWindow, QString target) {
}

void handling::knockCave(MainWindow *mainWindow, QString target) {
}

void handling::knockCaveEntrance(MainWindow *mainWindow, QString target) {
}

void handling::knockLake(MainWindow *mainWindow, QString target) {
}

void handling::knockValley(MainWindow *mainWindow, QString target) {
}
