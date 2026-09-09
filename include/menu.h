#ifndef MENU_H
#define MENU_H

typedef struct {
    double D;
    int N;
    char bNotation;
    double bVMax;
    double bX, bY;
    unsigned int seed;
} SimSettings;

void runMainMenu(void);

#endif
