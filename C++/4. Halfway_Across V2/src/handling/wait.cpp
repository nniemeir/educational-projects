#include "../../include/player.h"
#include "../../include/handling.h"
#include "../../include/world.h"

void handling::wait(MainWindow *mainWindow, Location* location) {
    if (location->getName() == "lake") {
        waitLake(mainWindow);
    }
}

void handling::waitLake(MainWindow *mainWindow) {
    if (world.getLineSet() == 1) {
        // Use probability to decide what is caught, if anything
        mainWindow->setDescription("You caught a fish!\n");
        player.addItem({"FISH", 1, 1, 15, "food"});
        world.setLineSet(0);
    }
}
