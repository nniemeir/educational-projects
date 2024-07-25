#include "lone.h"
// Different scenarios are triggered based on the animal encountered in a
// hunting encounter
char *generateAnimal() {
  float badgerProb = 5;
  float bearProb = 30;
  float deerProb = 70;
  float foxProb = 75;
  float rabbitProb = 90;
  static char animal[20];

  int prob = rand() % (100 + 1);
  if (prob <= badgerProb) {
    strcpy(animal, "badger");
  } else if (prob > badgerProb && prob <= bearProb) {
    strcpy(animal, "bear");
  } else if (prob > bearProb && prob <= deerProb) {
    strcpy(animal, "deer");
  } else if (prob > deerProb && prob <= foxProb) {
    strcpy(animal, "fox");
  } else if (prob > foxProb && prob <= rabbitProb) {
    strcpy(animal, "rabbit");
  } else if (prob > rabbitProb) {
    strcpy(animal, "squirrel");
  } else {
    printf("There was an issue deciding what animal type should be "
           "encountered...");
    return NULL;
  }
  return animal;
}