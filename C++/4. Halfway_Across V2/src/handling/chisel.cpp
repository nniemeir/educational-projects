#include "../../include/handling.h"
#include "../../include/player.h"
#include "../../include/world.h"

void handling::chisel(MainWindow *mainWindow, QString target, QString location) {
    if (location == "lake") {
        mainWindow->setDescription("You chisel a hole in the ice.\n");
    } else {
        mainWindow->setDescription("You can't chisel here.");
    }
}
