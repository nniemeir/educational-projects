#include "../../include/handling.h"
#include "../../include/player.h"
#include "../../include/world.h"

void handling::give(MainWindow *mainWindow, QString target, QString location) {
    if (location == "camp") {
        giveCamp(mainWindow, target);
    } else if (location == "campPath") {
        giveCampPath(mainWindow, target);
    } else if (location == "cave") {
        giveCave(mainWindow, target);
    } else if (location == "caveEntrance") {
        giveCaveEntrance(mainWindow, target);
    } else if (location == "lake") {
        giveLake(mainWindow, target);
    } else if (location == "valley") {
        giveValley(mainWindow, target);
    }
    else {
        mainWindow->setDescription("You can't do that here.");
    }
}

void handling::giveCamp(MainWindow *mainWindow, QString target) {

}

void handling::giveCampPath(MainWindow *mainWindow, QString target) {
}

void handling::giveCave(MainWindow *mainWindow, QString target) {
}

void handling::giveCaveEntrance(MainWindow *mainWindow, QString target) {
}

void handling::giveLake(MainWindow *mainWindow, QString target) {
}

void handling::giveValley(MainWindow *mainWindow, QString target) {
}
