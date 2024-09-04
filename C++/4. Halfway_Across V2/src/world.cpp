#include "../include/world.h"
#include "../include/player.h"
#include <QDebug>

worldStats world;

worldStats::worldStats()
    : currentLocation(nullptr), day(1), currentTemperature(32),
      currentWeather("clear"), chiseledIce(0),
      fish{{"CUTTHROAT TROUT", 1, 0, 25, "food"},
           {"BROOK TROUT", 1, 0, 25, "food"},
           {"RAINBOW TROUT", 1, 0, 30, "food"},
           {"BROWN TROUT", 1, 0, 35, "food"}}, directions{"NORTH", "WEST", "SOUTH", "EAST"} {}

bool worldStats::validDirection(const QString &value)
{
    return std::find(directions.begin(), directions.end(), value) != directions.end();
}

void worldStats::advanceDay() {
  player.setEnergy(1);
  world.setChiseledIce(0);
  day++;
  currentTemperature = generateTemperature();
  currentWeather = generateWeather();
}

void worldStats::initializeLocation(Location *initialLocation) {
  currentLocation = initialLocation;
}

int worldStats::getDay() const { return day; }

Location *worldStats::getCurrentLocation() const { return currentLocation; }

void worldStats::setCurrentLocation(Location *location) {
  currentLocation = location;
}

int worldStats::getChiseledIce() const { return chiseledIce; }

void worldStats::setChiseledIce(int newValue) { chiseledIce = newValue; }

int worldStats::getLineSet() const { return lineSet; }

void worldStats::setLineSet(int newValue) { lineSet = newValue; }

int worldStats::travelChecks() {
  if (currentWeather == "snowing heavily") {
    return TRAVEL_BLIZZARD;
  } else if (player.getEnergy() == 0) {
    return TRAVEL_TIRED;
  } else {
    return TRAVEL_YES;
  }
}

int worldStats::getCurrentTemperature() const { return currentTemperature; }

QString worldStats::getCurrentWeather() const { return currentWeather; }

// A given day's temperature will have an effect on
// how high the warmth stat must be to avoid losing health
int worldStats::generateTemperature() {
  int temperature = rand() % (30 + 1);
  return temperature;
}

// A given day's weather will have an effect on
// what actions the player can take and how likely they are to succeed at them
QString worldStats::generateWeather() {
  float blizzardProb = 5;
  float snowyProb = 20;
  float cloudyProb = 50;
  int prob = rand() % 100;
  if (prob < blizzardProb) {
    return "snowing heavily";
  } else if (prob < snowyProb) {
    return "snowing";
  } else if (prob < cloudyProb) {
    return "cloudy";
  } else {
    return "clear";
  }
}

QString worldStats::getFishName(int index) const { return fish[index].name; }

int worldStats::getFishEffect(int index) const { return fish[index].effects; }

int worldStats::getFishQuantity(int index) const { return fish[index].amount; }

const item &worldStats::getFish(int index) const { return fish[index]; }

int worldStats::searchFish(const QString &itemName) const {
  auto it = std::find_if(fish.begin(), fish.end(), [&](const item &i) {
    return i.name == itemName.toUpper();
  });
  if (it != fish.end()) {
    int index = std::distance(fish.begin(), it);
    return index;
  } else {
    qDebug() << "Generated fish was:" << itemName;
    return -1;
  }
}

QString worldStats::generateFish() {
  float brownTroutProb = 10;
  float rainbowTroutProb = 30;
  float brookTroutProb = 60;
  int prob = rand() % 100;
  if (prob < brownTroutProb) {
    return "BROWN TROUT";
  } else if (prob < rainbowTroutProb) {
    return "RAINBOW TROUT";
  } else if (prob < brookTroutProb) {
    return "BROOK TROUT";
  } else {
    return "CUTTHROAT TROUT";
  }
}
