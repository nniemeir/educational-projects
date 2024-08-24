#include "../include/handling.h"
#include "../include/player.h"
#include "../include/world.h"

const char *argVerbs[NUM_OF_ARG_VERBS] = {
    "ASK",     "ATTACK", "BURN", "CLOSE", "DRINK", "DROP",  "EAT",
    "EXAMINE", "GIVE",   "GO",   "KNOCK", "LOOK",  "MOVE",  "OPEN",
    "READ",    "SHOOT",  "SIT",  "TAKE",  "TELL",  "THROW", "USE"};

const char *noArgVerbs[NUM_OF_NO_ARG_VERBS] = {"BEGIN", "QUIT", "REFLECT", "SLEEP",
                                               "WAIT"};

handling handle;

// Inputted verb must match an element of argVerbs or noArgVerbs to be considered valid
bool handling::validateVerb(QString input) {
    bool validArgVerb = false;
    bool validNoArgVerb = false;
    QString inputVerb = input.split(" ").at(0);
    for (int i = 0; i < NUM_OF_ARG_VERBS; i++) {
        if (inputVerb == argVerbs[i]) {
            validArgVerb = true;
        }
    }
    for (int i = 0; i < NUM_OF_NO_ARG_VERBS; i++) {
        if (inputVerb == noArgVerbs[i]) {
            validNoArgVerb = true;
        }
    }

    if (validArgVerb || validNoArgVerb) {
        return true;
    }
    return false;
}

void handling::splitInput(MainWindow *mainWindow, QString input) {
    QStringList parts = input.split(" ");

    QString verb;
    QString target;

    if (!parts.isEmpty()) {
        verb = parts.at(0);

        if (parts.size() > 1) {
            target = parts.mid(1).join(" ");
        }
    }
    if (target == "") {
        mainWindow->setDescription(
            QString("You don't know what to %1.").arg(verb.toLower()));
    }

    handle.handleVerb(mainWindow, verb, target, world.currentLocation);
}

void handling::handleVerb(MainWindow *mainWindow, QString verb, QString target,
                          QString location) {
    if (verb == "BEGIN") {
        handling::begin(mainWindow, target, location);
    }
    else if (verb == "LOOK") {
        handling::look(mainWindow, target, location);
    } else if (verb == "GO" || verb == "MOVE") {
        handling::move(mainWindow, target, location);
    } else if (verb == "SIT") {
        handling::sit(mainWindow, target, location);
    } else if (verb == "SLEEP") {
        handling::sleep(mainWindow, target, location);
    } else if (verb == "USE") {
        handling::use(mainWindow, target, location);
    } else if (verb == "REFLECT") {
        mainWindow->setDescription(player.constructReflection());
    } else if (verb == "QUIT") {
        mainWindow->closeProgram();
    } else {
        mainWindow->setDescription(
            QString("You can't %1 here.").arg(verb.toLower()));
    }
}

void handling::begin(MainWindow *mainWindow, QString target, QString location) {
    if (location == "intro") {
        mainWindow->setLocation(camp);
    }
}

void handling::look(MainWindow *mainWindow, QString target, QString location) {
    if (location == "camp") {
        lookCamp(mainWindow, target);
    }
}

void handling::lookCamp(MainWindow *mainWindow, QString target) {
    if (target == "AROUND") {
        mainWindow->setDescription(camp.description);
    } else if (target == "AT FIRE") {
        mainWindow->setDescription("The fire burns low.");
    } else if (target == "AT BED") {
        mainWindow->setDescription("The bed seems to be quite worn.");
    } else if (target == "AT CHEST") {
        mainWindow->setDescription("The rusty chest contains your belongings.");
    } else if (target == "OUTSIDE") {
        mainWindow->setDescription(QString("It is %1 outside.").arg(world.currentWeather.toLower()));
    }
}

void handling::move(MainWindow *mainWindow, QString target, QString location) {
    if (location == "camp") {
        moveCamp(mainWindow, target);
    } else if (location == "campPath") {
        moveCampPath(mainWindow, target);
    } else if (location == "river") {
        moveRiver(mainWindow, target);
    } else if (location == "valley") {
        moveValley(mainWindow, target);
    } else if (location == "caveEntrance") {
        moveCaveEntrance(mainWindow, target);
    } else if (location == "cave") {
        moveCave(mainWindow, target);
    } else {
        qDebug() << "Error ascertaining location";
    }
}

void handling::moveCamp(MainWindow *mainWindow, QString target) {
    if (target == "NORTH") {
        int canTravel = world.travelChecks();
        switch (canTravel) {
        case TRAVEL_BLIZZARD:
            mainWindow->setDescription(
                "You can't risk traveling in such a blizzard.\n");
            break;
        case TRAVEL_TIRED:
            mainWindow->setDescription(
                "You do not have the energy to travel anywhere else today.\n");
            break;
        case TRAVEL_YES:
            mainWindow->setLocation(campPath);
            break;
        }
    }
}

void handling::moveCampPath(MainWindow *mainWindow, QString target) {
    if (target == "WEST") {
        mainWindow->setLocation(river);
        player.energy = 0;
    } else if (target == "EAST") {
        mainWindow->setLocation(caveEntrance);
        player.energy = 0;
    } else if (target == "NORTH") {
        mainWindow->setLocation(valley);
        player.energy = 0;
    } else if (target == "SOUTH") {
        mainWindow->setLocation(camp);
    }
}

void handling::moveCave(MainWindow *mainWindow, QString target) {
    if (target == "WEST") {
        mainWindow->setLocation(caveEntrance);
    }
}

void handling::moveCaveEntrance(MainWindow *mainWindow, QString target) {
    if (target == "WEST") {
        mainWindow->setLocation(campPath);
    }
    else if (target == "EAST") {
        mainWindow->setLocation(cave);
    }
}

void handling::moveRiver(MainWindow *mainWindow, QString target) {
    if (target == "EAST") {
        mainWindow->setLocation(campPath);
    }
}

void handling::moveValley(MainWindow *mainWindow, QString target) {
    if (target == "SOUTH") {
        mainWindow->setLocation(campPath);
    }
}

void handling::sit(MainWindow *mainWindow, QString target, QString location) {
    if (location == "camp") {
        if (target == "ON LOG") {
            mainWindow->setDescription("You feel a bit warmer sitting by the fire.");
        }
    }
}

void handling::sleep(MainWindow *mainWindow, QString target, QString location) {
    if (location == "camp") {
        mainWindow->setDescription("You close your eyes and another night passes.\n"),
            world.advanceDay();
    }
}

void handling::use(MainWindow *mainWindow, QString target, QString location) {
    if (location == "camp") {
        if (target == "CHEST") {
            mainWindow->setDescription("Inventory menu will go here.\n");
        }
    }
}
