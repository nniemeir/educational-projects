#include "../include/halfway_across.h"

void advanceDay() {
  player.energy = 1;
  world.day = world.day + 1;
  world.currentTemperature = generateTemperature();
  memset(world.currentWeather, 0, sizeof(world.currentWeather));
  snprintf(world.currentWeather, sizeof(world.currentWeather), "%s", generateWeather());
  
  return;
}

// A given day's temperature will have an effect on
// how high the warmth stat must be to avoid losing health
int generateTemperature() {
  int currentTemperature = rand() % (30 + 1);
  return currentTemperature;
}

// A given day's weather will have an effect on
// what actions the player can take and how likely they are to succeed at them
char *generateWeather() {
  float blizzardProb = 5;
  float snowyProb = 20;
  float cloudyProb = 50;
  static char currentWeather[20];

  int prob = rand() % 100;
  if (prob <= blizzardProb) {
    snprintf(currentWeather, sizeof(currentWeather), "snowed heavily");
  } else if (prob > blizzardProb && prob <= snowyProb) {
    snprintf(currentWeather, sizeof(currentWeather), "snowed");
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

void travelChecks() {
  if (strcmp(world.currentWeather, "snowed heavily") == 0) {
    clearScreen();
    printf("It was snowing too heavily for me to travel...");
    clearStdin();
    clearScreen();
  } else if (player.energy == 0) {
    clearScreen();
    printf("I didn't have the energy to travel anywhere else today...");
    clearStdin();
    clearScreen();
  } else {
    travelMenu();
  }
}