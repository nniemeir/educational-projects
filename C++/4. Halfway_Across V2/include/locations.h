#ifndef LOCATIONS_H
#define LOCATIONS_H
#include "QString"

class Location {
public:
    QString name;
    QString description;
    QString image;
};

extern Location camp;
extern Location campPath;
extern Location cave;
extern Location caveEntrance;
extern Location intro;
extern Location valley;
extern Location river;

#endif // LOCATIONS_H
