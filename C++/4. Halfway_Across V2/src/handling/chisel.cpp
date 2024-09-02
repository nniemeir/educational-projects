#include "../../include/handling.h"
#include "../../include/world.h"
#include "../../include/player.h"

void handling::chisel(MainWindow *mainWindow, QString target,
                      Location* location) {
    if (location->getName() == "lake") {
        if (player.searchInventory("CHISEL") != -1) {
    mainWindow->setDescription("You chisel a hole in the ice.\n");
    world.setChiseledIce(1);
        } else {
            mainWindow->setDescription("You don't have a chisel.");
        }
  } else {
    mainWindow->setDescription("You can't chisel here.");
  }
}
