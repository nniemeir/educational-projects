#include "../include/halfway_across.h"

#define LOW_STAT_THRESHOLD 20
#define MEDIUM_STAT_THRESHOLD 50
#define HIGH_STAT_THRESHOLD 70

struct world_stats world = {1, 32, "clear"};

struct player_stats player = {100, 1, 100, 100, 50, 50};

// Gives player vague indication of stats if they select "Reflect" option
void warnings(void) {
  clear_screen();
  if (player.warmth < LOW_STAT_THRESHOLD) {
    printf("I won't survive this cold much longer.\n");
  } else if (LOW_STAT_THRESHOLD <= player.warmth &&
             player.warmth < MEDIUM_STAT_THRESHOLD) {
    printf("The cold is becoming unbearable.\n");
  } else if (MEDIUM_STAT_THRESHOLD <= player.warmth &&
             player.warmth < HIGH_STAT_THRESHOLD) {
    printf("The cold is starting to get to me.\n");
  }

  if (player.health < LOW_STAT_THRESHOLD) {
    printf("My health is quickly deteriorating.\n");
  } else if (LOW_STAT_THRESHOLD <= player.health &&
             player.health < MEDIUM_STAT_THRESHOLD) {
    printf("I feel very ill.\n");
  } else if (MEDIUM_STAT_THRESHOLD <= player.health &&
             player.health < HIGH_STAT_THRESHOLD) {
    printf("I feel a little under the currentWeather.\n");
  }

  if (player.hunger < LOW_STAT_THRESHOLD) {
    printf("I am starving.\n");
  } else if (LOW_STAT_THRESHOLD <= player.hunger &&
             player.hunger < MEDIUM_STAT_THRESHOLD) {
    printf("I need to eat something soon.\n");
  } else if (MEDIUM_STAT_THRESHOLD <= player.hunger &&
             player.hunger < HIGH_STAT_THRESHOLD) {
    printf("I am starting to get hungry.\n");
  }

  if (player.thirst < LOW_STAT_THRESHOLD) {
    printf("My throat is very dry.\n");
  } else if (LOW_STAT_THRESHOLD <= player.thirst &&
             player.thirst < MEDIUM_STAT_THRESHOLD) {
    printf("I need to drink something soon.\n");
  } else if (MEDIUM_STAT_THRESHOLD <= player.thirst &&
             player.thirst < HIGH_STAT_THRESHOLD) {
    printf("I am starting to get thirsty.\n");
  }
  printf("\nI will persist...");
  clear_stdin();
}
