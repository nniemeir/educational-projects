#include "../../include/handling.h"
#include "../../include/player.h"
#include "../../include/world.h"

void handling::ask(MainWindow *mainWindow, QString target, QString location) {
    if (location == "camp") {
        askCamp(mainWindow, target);
    } else if (location == "campPath") {
        askCampPath(mainWindow, target);
    } else if (location == "cave") {
        askCave(mainWindow, target);
    } else if (location == "caveEntrance") {
        askCaveEntrance(mainWindow, target);
    } else if (location == "lake") {
        askLake(mainWindow, target);
    } else if (location == "valley") {
        askValley(mainWindow, target);
    }
    else {
        mainWindow->setDescription("You can't do that here.");
    }
}

void handling::askCamp(MainWindow *mainWindow, QString target) {

}

void handling::askCampPath(MainWindow *mainWindow, QString target) {
}

void handling::askCave(MainWindow *mainWindow, QString target) {
}

void handling::askCaveEntrance(MainWindow *mainWindow, QString target) {
}

void handling::askLake(MainWindow *mainWindow, QString target) {
}

void handling::askValley(MainWindow *mainWindow, QString target) {
}
