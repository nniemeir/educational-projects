#include "../../include/handling.h"
#include "../../include/player.h"
#include "../../include/world.h"

void handling::tell(MainWindow *mainWindow, QString target, QString location) {
    if (location == "camp") {
        tellCamp(mainWindow, target);
    } else if (location == "campPath") {
        tellCampPath(mainWindow, target);
    } else if (location == "cave") {
        tellCave(mainWindow, target);
    } else if (location == "caveEntrance") {
        tellCaveEntrance(mainWindow, target);
    } else if (location == "lake") {
        tellLake(mainWindow, target);
    } else if (location == "valley") {
        tellValley(mainWindow, target);
    }
    else {
        mainWindow->setDescription("You can't do that here.");
    }
}

void handling::tellCamp(MainWindow *mainWindow, QString target) {

}

void handling::tellCampPath(MainWindow *mainWindow, QString target) {
}

void handling::tellCave(MainWindow *mainWindow, QString target) {
}

void handling::tellCaveEntrance(MainWindow *mainWindow, QString target) {
}

void handling::tellLake(MainWindow *mainWindow, QString target) {
}

void handling::tellValley(MainWindow *mainWindow, QString target) {
}
