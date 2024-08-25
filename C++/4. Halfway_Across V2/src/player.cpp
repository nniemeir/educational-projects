#include "../include/player.h"
#include "../include/world.h"

#define LOW_STAT_THRESHOLD 20
#define MEDIUM_STAT_THRESHOLD 50
#define HIGH_STAT_THRESHOLD 70

playerStats player = {
    100, 1, 100, 100, 50, 50
};

int playerStats::constrainStat(int stat) {
    return std::max(0, std::min(stat, 100));
}

QString playerStats::constructReflection() {
    QString reflection;
    reflection.append(QString("Day %1 in the San Juan mountains.\n").arg(world.day));

    if (player.warmth < LOW_STAT_THRESHOLD) {
        reflection.append("You won't survive this cold much longer.\n");
    } else if (LOW_STAT_THRESHOLD <= player.warmth &&
               player.warmth < MEDIUM_STAT_THRESHOLD) {
        reflection.append("The cold is becoming unbearable.\n");
    } else if (MEDIUM_STAT_THRESHOLD <= player.warmth &&
               player.warmth < HIGH_STAT_THRESHOLD) {
        reflection.append("The cold is starting to get to you.\n");
    }

    if (player.health < LOW_STAT_THRESHOLD) {
        reflection.append("Your health is deteriorating quickly.\n");
    } else if (LOW_STAT_THRESHOLD <= player.health &&
               player.health < MEDIUM_STAT_THRESHOLD) {
        reflection.append("You feel very ill.\n");
    } else if (MEDIUM_STAT_THRESHOLD <= player.health &&
               player.health < HIGH_STAT_THRESHOLD) {
        reflection.append("You feel a little under the currentWeather.\n");
    }

    if (player.hunger < LOW_STAT_THRESHOLD) {
        reflection.append("You are starving.\n");
    } else if (LOW_STAT_THRESHOLD <= player.hunger &&
               player.hunger < MEDIUM_STAT_THRESHOLD) {
        reflection.append("You need to eat something soon.\n");
    } else if (MEDIUM_STAT_THRESHOLD <= player.hunger &&
               player.hunger < HIGH_STAT_THRESHOLD) {
        reflection.append("You are starting to get hungry.\n");
    }

    if (player.thirst < LOW_STAT_THRESHOLD) {
        reflection.append("Your throat is very dry.\n");
    } else if (LOW_STAT_THRESHOLD <= player.thirst &&
               player.thirst < MEDIUM_STAT_THRESHOLD) {
        reflection.append("You need to drink something soon.\n");
    } else if (MEDIUM_STAT_THRESHOLD <= player.thirst &&
               player.thirst < HIGH_STAT_THRESHOLD) {
        reflection.append("You are starting to get thirsty.\n");
    }
    return reflection;
}
