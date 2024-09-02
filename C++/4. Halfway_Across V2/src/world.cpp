#include "../include/world.h"
#include "../include/player.h"

worldStats world;

worldStats::worldStats()
    : currentLocation(nullptr), day(1), currentTemperature(32),
      currentWeather("clear"), chiseledIce(0) {}

void worldStats::advanceDay() {
  player.setEnergy(1);
  world.setChiseledIce(0);
  day++;
  currentTemperature = generateTemperature();
  currentWeather = generateWeather();
}

void worldStats::initializeLocation(Location* initialLocation) {
    currentLocation = initialLocation;
}

int worldStats::getDay() const { return day; }

Location* worldStats::getCurrentLocation() const { return currentLocation; }

void worldStats::setCurrentLocation(Location* location) {
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
  QString weather;
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
