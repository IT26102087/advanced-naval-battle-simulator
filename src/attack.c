#include <math.h>
#include "../include/common.h"
#include "../include/attack.h"
#include "../include/physics.h"

double battleshipMaxRange(Battleship *b) {
    return (b->vMax * b->vMax) / G;
}

int isInBattleshipRange(Battleship *b, Escortship *e) {
    double d = distanceBetween(b->x, b->y, e->x, e->y);
    return (d <= battleshipMaxRange(b)) ? 1 : 0;
}

static double sin2theta(double thetaDeg) {
    double thetaRad = thetaDeg * PI / 180.0;
    return sin(2.0 * thetaRad);
}

double escortMaxRange(Escortship *e) {
    double maxSin2;
    if (e->angleMin <= 45.0 && e->angleMax >= 45.0) {
        maxSin2 = 1.0;
    } else {
        double s1 = sin2theta(e->angleMin);
        double s2 = sin2theta(e->angleMax);
        maxSin2 = (s1 > s2) ? s1 : s2;
    }
    return (e->vMax * e->vMax * maxSin2) / G;
}

double escortMinRange(Escortship *e) {
    double s1 = sin2theta(e->angleMin);
    double s2 = sin2theta(e->angleMax);
    double minSin2 = (s1 < s2) ? s1 : s2;
    return (e->vMin * e->vMin * minSin2) / G;
}

int isInEscortRange(Escortship *e, Battleship *b) {
    double d = distanceBetween(e->x, e->y, b->x, b->y);
    double rMin = escortMinRange(e);
    double rMax = escortMaxRange(e);
    return (d >= rMin && d <= rMax) ? 1 : 0;
}
