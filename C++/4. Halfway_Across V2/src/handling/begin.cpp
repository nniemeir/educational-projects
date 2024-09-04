#include "../../include/audio.h"
#include "../../include/handling.h"
#include "../../include/player.h"
#include "../../include/world.h"

void handling::begin(MainWindow *mainWindow, QString target,
                     Location *location) {
  if (location->getName() == "intro") {
    sfxPlayer.play("qrc:/audio/sfx/lanternRustle.mp3", 0);

    mainWindow->setLocation(intro.getMusicPath(), intro.getAmbiencePath(),
                            &camp);
  } else {
    mainWindow->setDescription("You can't do that here.");
  }
}
