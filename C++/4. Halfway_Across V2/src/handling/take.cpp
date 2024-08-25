#include "../../include/handling.h"
#include "../../include/player.h"
#include "../../include/world.h"
void handling::take(MainWindow *mainWindow, QString target, QString location) {
    if (location == "camp") {
        takeCamp(mainWindow, target);
    } else if (location == "campPath") {
        takeCampPath(mainWindow, target);
    } else if (location == "cave") {
        takeCave(mainWindow, target);
    } else if (location == "caveEntrance") {
        takeCaveEntrance(mainWindow, target);
    } else if (location == "lake") {
        takeLake(mainWindow, target);
    } else if (location == "valley") {
        takeValley(mainWindow, target);
    }
    else {
        mainWindow->setDescription("You can't do that here.");
    }
}

void handling::takeCamp(MainWindow *mainWindow, QString target) {

}

void handling::takeCampPath(MainWindow *mainWindow, QString target) {
}

void handling::takeCave(MainWindow *mainWindow, QString target) {
}

void handling::takeCaveEntrance(MainWindow *mainWindow, QString target) {
}

void handling::takeLake(MainWindow *mainWindow, QString target) {
}

void handling::takeValley(MainWindow *mainWindow, QString target) {
}
