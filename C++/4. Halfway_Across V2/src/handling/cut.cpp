#include "../../include/handling.h"
#include "../../include/player.h"
#include "../../include/world.h"

void handling::cut(MainWindow *mainWindow, QString target, QString location) {
    if (location == "camp") {
        cutCamp(mainWindow, target);
    } else if (location == "campPath") {
        cutCampPath(mainWindow, target);
    } else if (location == "cave") {
        cutCave(mainWindow, target);
    } else if (location == "caveEntrance") {
        cutCaveEntrance(mainWindow, target);
    } else if (location == "lake") {
        cutLake(mainWindow, target);
    } else if (location == "valley") {
        cutValley(mainWindow, target);
    }
    else {
        mainWindow->setDescription("You can't do that here.");
    }
}

void handling::cutCamp(MainWindow *mainWindow, QString target) {

}

void handling::cutCampPath(MainWindow *mainWindow, QString target) {
}

void handling::cutCave(MainWindow *mainWindow, QString target) {
}

void handling::cutCaveEntrance(MainWindow *mainWindow, QString target) {
}

void handling::cutLake(MainWindow *mainWindow, QString target) {
}

void handling::cutValley(MainWindow *mainWindow, QString target) {
}
