#include "../../include/handling.h"
#include "../../include/player.h"

void handling::remove(MainWindow *mainWindow, QString target) {
  int index = player.searchInventory(target.toUpper());
  if (index != -1) {
    if (player.getItemEquipped(index) == 1) {
      player.setItemEquipped(index, 0);
      player.setWarmth(player.getWarmth() - player.getItemEffect(index));
      mainWindow->setDescription(
          QString("You remove your %1.").arg(target.toLower()));
    } else {
      mainWindow->setDescription(
          QString("You are not wearing your %1.").arg(target.toLower()));
    }
  } else {
    mainWindow->setDescription("You do not possess that.");
  }
}
