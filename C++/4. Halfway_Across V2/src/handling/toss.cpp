#include "../../include/handling.h"
#include "../../include/player.h"
#include "../../include/world.h"

void handling::toss(MainWindow *mainWindow, QString target, QString location) {
    if (location == "camp") {
        tossCamp(mainWindow, target);
    } else if (location == "campPath") {
        tossCampPath(mainWindow, target);
    } else if (location == "cave") {
        tossCave(mainWindow, target);
    } else if (location == "caveEntrance") {
        tossCaveEntrance(mainWindow, target);
    } else if (location == "lake") {
        tossLake(mainWindow, target);
    } else if (location == "valley") {
        tossValley(mainWindow, target);
    }
    else {
        mainWindow->setDescription("You can't do that here.");
    }
}

void handling::tossCamp(MainWindow *mainWindow, QString target) {

}

void handling::tossCampPath(MainWindow *mainWindow, QString target) {
}

void handling::tossCave(MainWindow *mainWindow, QString target) {
}

void handling::tossCaveEntrance(MainWindow *mainWindow, QString target) {
}

void handling::tossLake(MainWindow *mainWindow, QString target) {
}

void handling::tossValley(MainWindow *mainWindow, QString target) {
}
