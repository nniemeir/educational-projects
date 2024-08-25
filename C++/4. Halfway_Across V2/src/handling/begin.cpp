#include "../../include/handling.h"
#include "../../include/player.h"
#include "../../include/world.h"

void handling::begin(MainWindow *mainWindow, QString target, QString location) {
    if (location == "intro") {
        mainWindow->setLocation(camp);
    } else {
        mainWindow->setDescription("You can't do that here.");
    }
}
