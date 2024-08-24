#ifndef HANDLING_H
#define HANDLING_H
#include "../include/mainwindow.h"

#define NUM_OF_ARG_VERBS 21
#define NUM_OF_NO_ARG_VERBS 5

class handling {
public:
    bool validateVerb(QString input);
    void splitInput(MainWindow *mainWindow, QString input);
private:
    void handleVerb(MainWindow *mainWindow, QString verb, QString target, QString location);
    void begin(MainWindow *mainWindow, QString target, QString location);
    void look(MainWindow *mainWindow, QString target, QString location);
    void lookCamp(MainWindow *mainWindow, QString target);
    void move(MainWindow *mainWindow, QString target, QString location);
    void moveCamp(MainWindow *mainWindow, QString target);
    void moveCampPath(MainWindow *mainWindow, QString target);
    void moveRiver(MainWindow *mainWindow, QString target);
    void moveValley(MainWindow *mainWindow, QString target);
    void moveCaveEntrance(MainWindow *mainWindow, QString target);
    void moveCave(MainWindow *mainWindow, QString target);
    void sit(MainWindow *mainWindow, QString target, QString location);
    void sleep(MainWindow *mainWindow, QString target, QString location);
    void use(MainWindow *mainWindow, QString target, QString location);
};
extern handling handle;

extern const char *argVerbs[NUM_OF_ARG_VERBS];
extern const char *noArgVerbs[NUM_OF_NO_ARG_VERBS];

#endif // HANDLING_H
