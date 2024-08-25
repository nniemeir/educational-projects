#include "../../include/handling.h"
#include "../../include/player.h"
#include "../../include/world.h"

void handling::use(MainWindow *mainWindow, QString target, QString location) {
    if (location == "camp") {
        useCamp(mainWindow, target);
    } else if (location == "campPath") {
        useCampPath(mainWindow, target);
    } else if (location == "cave") {
        useCave(mainWindow, target);
    } else if (location == "caveEntrance") {
        useCaveEntrance(mainWindow, target);
    } else if (location == "lake") {
        useLake(mainWindow, target);
    } else if (location == "valley") {
        useValley(mainWindow, target);
    }
    else {
        mainWindow->setDescription("You can't do that here.");
    }
}

void handling::useCamp(MainWindow *mainWindow, QString target) {
}

void handling::useCampPath(MainWindow *mainWindow, QString target) {
}

void handling::useCave(MainWindow *mainWindow, QString target) {
}

void handling::useCaveEntrance(MainWindow *mainWindow, QString target) {
}

void handling::useLake(MainWindow *mainWindow, QString target) {
}

void handling::useValley(MainWindow *mainWindow, QString target) {
}
