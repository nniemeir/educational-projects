#include "../../include/handling.h"
#include "../../include/player.h"

void handling::wear(MainWindow *mainWindow, QString target) {
  int index = player.searchInventory(target.toUpper());
  if (index != -1) {
    if (player.getItemEquipped(index) == 0) {
      player.setItemEquipped(index, 1);
      qDebug() << "Warmth before equip is" << player.getWarmth();
      player.setWarmth(player.getWarmth() + player.getItemEffect(index));
      qDebug() << "Warmth after equip is" << player.getWarmth();
      mainWindow->setDescription(
          QString("You put on your %1.").arg(target.toLower()));
    } else {
      mainWindow->setDescription(
          QString("You are already wearing your %1.").arg(target.toLower()));
    }
  } else {
    mainWindow->setDescription("You do not possess that.");
  }
}
