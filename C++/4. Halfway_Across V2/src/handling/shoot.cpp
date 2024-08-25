#include "../../include/handling.h"
#include "../../include/player.h"
#include "../../include/world.h"

void handling::shoot(MainWindow *mainWindow, QString target, QString location) {
    if (location == "camp") {
        shootCamp(mainWindow, target);
    } else if (location == "campPath") {
        shootCampPath(mainWindow, target);
    } else if (location == "cave") {
        shootCave(mainWindow, target);
    } else if (location == "caveEntrance") {
        shootCaveEntrance(mainWindow, target);
    } else if (location == "lake") {
        shootLake(mainWindow, target);
    } else if (location == "valley") {
        shootValley(mainWindow, target);
    }
    else {
        mainWindow->setDescription("You can't do that here.");
    }
}

void handling::shootCamp(MainWindow *mainWindow, QString target) {

}

void handling::shootCampPath(MainWindow *mainWindow, QString target) {
}

void handling::shootCave(MainWindow *mainWindow, QString target) {
}

void handling::shootCaveEntrance(MainWindow *mainWindow, QString target) {
}

void handling::shootLake(MainWindow *mainWindow, QString target) {
}

void handling::shootValley(MainWindow *mainWindow, QString target) {
}
