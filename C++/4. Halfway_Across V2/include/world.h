#ifndef WORLD_H
#define WORLD_H
#include <time.h>
#include "QString"

class worldStats {
public:
    QString currentLocation;
    int day;
    int currentTemperature;
    QString currentWeather;
    void advanceDay();
    int travelChecks();
private:
    QString generateWeather();
    int generateTemperature();
};

typedef enum {
    TRAVEL_YES = 1,
    TRAVEL_BLIZZARD,
    TRAVEL_TIRED
} TravelResponses;

extern worldStats world;
#endif // WORLD_H
