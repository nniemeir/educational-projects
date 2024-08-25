#include "../../include/handling.h"
#include "../../include/player.h"
#include "../../include/world.h"

void handling::look(MainWindow *mainWindow, QString target, QString location) {
    if (location == "camp") {
        lookCamp(mainWindow, target);
    } else if (location == "campPath") {
        lookCampPath(mainWindow, target);
    } else if (location == "cave") {
        lookCave(mainWindow, target);
    } else if (location == "caveEntrance") {
        lookCaveEntrance(mainWindow, target);
    } else if (location == "lake") {
        lookLake(mainWindow, target);
    } else if (location == "valley") {
        lookValley(mainWindow, target);
    }
    else {
        mainWindow->setDescription("You can't do that here.");
    }
}


void handling::lookCamp(MainWindow *mainWindow, QString target) {
    static const QMap<QString, QString> descriptions = {
        { "AROUND", camp.description },
        { "AT FIRE", "The fire burns low." },
        { "AT BED", "The bed seems to be quite worn." },
        { "AT CHEST", "The rusty chest contains your belongings." },
        { "OUTSIDE", QString("It is %1 outside.").arg(world.currentWeather.toLower()) }
    };

    if (descriptions.contains(target)) {
        mainWindow->setDescription(descriptions.value(target));
    } else {
        mainWindow->setDescription("That is not an item you are aware of.\n");
    }
}

void handling::lookCampPath(MainWindow *mainWindow, QString target) {
}

void handling::lookCave(MainWindow *mainWindow, QString target) {
}

void handling::lookCaveEntrance(MainWindow *mainWindow, QString target) {
}

void handling::lookLake(MainWindow *mainWindow, QString target) {
}

void handling::lookValley(MainWindow *mainWindow, QString target) {
}
