#include "../../include/handling.h"
#include "../../include/player.h"
#include "../../include/world.h"

void handling::examine(MainWindow *mainWindow, QString target, QString location) {
    if (location == "camp") {
        examineCamp(mainWindow, target);
    } else if (location == "campPath") {
        examineCampPath(mainWindow, target);
    } else if (location == "cave") {
        examineCave(mainWindow, target);
    } else if (location == "caveEntrance") {
        examineCaveEntrance(mainWindow, target);
    } else if (location == "lake") {
        examineLake(mainWindow, target);
    } else if (location == "valley") {
        examineValley(mainWindow, target);
    }
    else {
        mainWindow->setDescription("You can't do that here.");
    }
}

void handling::examineCamp(MainWindow *mainWindow, QString target) {

}

void handling::examineCampPath(MainWindow *mainWindow, QString target) {
}

void handling::examineCave(MainWindow *mainWindow, QString target) {
}

void handling::examineCaveEntrance(MainWindow *mainWindow, QString target) {
}

void handling::examineLake(MainWindow *mainWindow, QString target) {
}

void handling::examineValley(MainWindow *mainWindow, QString target) {
}
