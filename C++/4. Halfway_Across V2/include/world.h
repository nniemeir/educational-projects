#ifndef WORLD_H
#define WORLD_H
#include "halfway.h"
#include <time.h>
#include "../include/locations.h"


class worldStats {
public:
  worldStats();

  // Day
  void advanceDay();
  int getDay() const;

  // Location
  Location* getCurrentLocation() const;
  void initializeLocation(Location* initialLocation);

  void setCurrentLocation(Location* location);

  // Day-specific
  int getCurrentTemperature() const;
  QString getCurrentWeather() const;
  int getChiseledIce() const;
  void setChiseledIce(int newValue);
  int getLineSet() const;
  void setLineSet(int newValue);
  int travelChecks();

private:
  int day;
  Location* currentLocation;
  int currentTemperature;
  QString currentWeather;
  int chiseledIce;
  int lineSet;
  int generateTemperature();
  QString generateWeather();
};

typedef enum { TRAVEL_YES = 1, TRAVEL_BLIZZARD, TRAVEL_TIRED } TravelResponses;

extern worldStats world;
#endif // WORLD_H
