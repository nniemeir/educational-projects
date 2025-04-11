#include "../include/halfway_across.h"

void advance_day(void) {
  player.energy = 1;
  world.day = world.day + 1;
  world.current_temperature = generate_temperature();
  memset(world.current_weather, 0, sizeof(world.current_weather));
  snprintf(world.current_weather, sizeof(world.current_weather), "%s",
           generate_weather());
  return;
}

// A given day's temperature will have an effect on
// how high the warmth stat must be to avoid losing health
int generate_temperature(void) {
  int current_temperature = rand() % (30 + 1);
  return current_temperature;
}

// A given day's weather will have an effect on
// what actions the player can take and how likely they are to succeed at them
char *generate_weather(void) {
  float blizzard_prob = 5;
  float snowy_prob = 20;
  float cloudy_prob = 50;
  static char current_weather[20];

  int prob = rand() % 100;
  if (prob <= blizzard_prob) {
    snprintf(current_weather, sizeof(current_weather), "snowed heavily");
  } else if (prob > blizzard_prob && prob <= snowy_prob) {
    snprintf(current_weather, sizeof(current_weather), "snowed");
  } else if (prob > snowy_prob && prob <= cloudy_prob) {
    snprintf(current_weather, sizeof(current_weather), "cloudy");
  } else if (prob > cloudy_prob) {
    snprintf(current_weather, sizeof(current_weather), "clear");
  } else {
    fprintf(stderr, "Failed to generate weather conditions");
    return NULL;
  }
  return current_weather;
}

void travel_checks(void) {
  if (strcmp(world.current_weather, "snowed heavily") == 0) {
    clear_screen();
    printf("It was snowing too heavily for me to travel...");
    clear_stdin();
    clear_screen();
    return;
  }

  if (player.energy == 0) {
    clear_screen();
    printf("I didn't have the energy to travel anywhere else today...");
    clear_stdin();
    clear_screen();
    return;
  }

  travel_menu();
}