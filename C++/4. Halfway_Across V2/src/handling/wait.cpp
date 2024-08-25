#include "../../include/handling.h"
#include "../../include/player.h"
#include "../../include/world.h"

void handling::wait(MainWindow *mainWindow, QString target, QString location) {
    if (location == "camp") {
        waitCamp(mainWindow, target);
    } else if (location == "campPath") {
        waitCampPath(mainWindow, target);
    } else if (location == "cave") {
        waitCave(mainWindow, target);
    } else if (location == "caveEntrance") {
        waitCaveEntrance(mainWindow, target);
    } else if (location == "lake") {
        waitLake(mainWindow, target);
    } else if (location == "valley") {
        waitValley(mainWindow, target);
    }
    else {
        mainWindow->setDescription("You can't do that here.");
    }
}

void handling::waitCamp(MainWindow *mainWindow, QString target) {

}

void handling::waitCampPath(MainWindow *mainWindow, QString target) {
}

void handling::waitCave(MainWindow *mainWindow, QString target) {
}

void handling::waitCaveEntrance(MainWindow *mainWindow, QString target) {
}

void handling::waitLake(MainWindow *mainWindow, QString target) {
}

void handling::waitValley(MainWindow *mainWindow, QString target) {
}
