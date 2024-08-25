#include "../include/player.h"
#include "../include/world.h"
#include "QDebug"
worldStats world = {
    "intro",
    1,
    32,
    "clear"
};

void worldStats::advanceDay() {
    player.energy = 1;
    world.day = world.day + 1;
    world.currentTemperature = generateTemperature();
    world.currentWeather = QString("%1").arg(generateWeather());
    qDebug() << world.currentWeather;
    return;
}

// A given day's temperature will have an effect on
// how high the warmth stat must be to avoid losing health
int worldStats::generateTemperature() {
    int currentTemperature = rand() % (30 + 1);
    return currentTemperature;
}

// A given day's weather will have an effect on
// what actions the player can take and how likely they are to succeed at them
QString worldStats::generateWeather() {
    float blizzardProb = 5;
    float snowyProb = 20;
    float cloudyProb = 50;
    static char currentWeather[20];

    int prob = rand() % 100;
    if (prob <= blizzardProb) {
        snprintf(currentWeather, sizeof(currentWeather), "snowing heavily");
    } else if (prob > blizzardProb && prob <= snowyProb) {
        snprintf(currentWeather, sizeof(currentWeather), "snowing");
    } else if (prob > snowyProb && prob <= cloudyProb) {
        snprintf(currentWeather, sizeof(currentWeather), "cloudy");
    } else if (prob > cloudyProb) {
        snprintf(currentWeather, sizeof(currentWeather), "clear");
    } else {
        printf("There was an issue generating weather conditions");
        return NULL;
    }
    return currentWeather;
}

int worldStats::travelChecks() {
    if (world.currentWeather == "snowing heavily") {
        return TRAVEL_BLIZZARD;
    } else if (player.energy == 0) {
        return TRAVEL_TIRED;
    } else {
        return TRAVEL_YES;
    }
}

