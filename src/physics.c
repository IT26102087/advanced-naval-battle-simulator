#include <math.h>
#include "../include/common.h"
#include "../include/physics.h"

double timeOfFlight(double u, double thetaDeg) {
    double thetaRad = thetaDeg * PI / 180.0;
    double uy = u * sin(thetaRad);
    /* t_p (time to peak) = uy / g, total flight time t_f = 2 * t_p */
    return (2.0 * uy) / G;
}

double calcRange(double u, double thetaDeg) {
    double thetaRad = thetaDeg * PI / 180.0;
    return (u * u * sin(2.0 * thetaRad)) / G;
}

double distanceBetween(double x1, double y1, double x2, double y2) {
    double dx = x2 - x1;
    double dy = y2 - y1;
    return sqrt(dx * dx + dy * dy);
}
