#include "../../include/handling.h"
#include "../../include/player.h"
#include "../../include/world.h"

void handling::sleep(MainWindow *mainWindow, QString target, QString location) {
    if (location == "camp") {
        mainWindow->setDescription("You close your eyes and another night passes.\n"),
            world.advanceDay();
    } else {
        mainWindow->setDescription("You can't do that here.");
    }
}
