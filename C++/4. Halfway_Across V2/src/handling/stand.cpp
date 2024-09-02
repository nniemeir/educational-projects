#include "../../include/handling.h"
#include "../../include/player.h"

void handling::stand(MainWindow *mainWindow) {
  if (player.getStanding() == 0) {
    player.setStanding(1);
    mainWindow->setDescription("You stand up.\n");
  } else {
    mainWindow->setDescription("You are already standing.\n");
  }
}
