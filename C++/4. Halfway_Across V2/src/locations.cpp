#include "../include/locations.h"

Location camp = {"camp", "You wake up sore, that familiar ache of winter mornings felt once again.\n\nExit: North", ":/images/camp.png"};

Location campPath = {"campPath", "You move through a break in the trees for a while before reaching a fork in the road.\n\nExits: North, South, East, West",
                     ":/images/campPath.png"};

Location caveEntrance = {"caveEntrance", "You happen upon a cave, it might not be a good idea to find out what it contains.\n\nExits: West, East",
                     ":/images/caveEntrance.png"};

Location cave = {"cave", "Entering the cave, you see nothing but darkness.\n\nExits:West",
                         ":/images/cave.png"};
Location intro = {
    "intro",
    "You find a tattered journal at an abandoned camp in the mountains",
    ":/images/intro.png"};

Location river = {"river",
                  "You reach a river after a few hours journey.\n\nExit: East",
                  ":/images/river.png"};

Location valley = {"valley",
                   "You reach a clearing. This could be a good spot for hunting.\n\nExit: South",
                   ":/images/valley.png"};
