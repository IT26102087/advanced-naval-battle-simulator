#ifndef PHYSICS_H
#define PHYSICS_H

/* All angles in these functions are in DEGREES for readability;
   they get converted to radians internally. */

/* Time of flight for a shell launched at speed u, angle theta (deg),
   until it lands back at the same height (y = 0).                 */
double timeOfFlight(double u, double thetaDeg);

/* Horizontal range (distance) travelled: R = u^2 * sin(2*theta) / g */
double calcRange(double u, double thetaDeg);

/* Straight-line (Euclidean) distance between two points on the canvas */
double distanceBetween(double x1, double y1, double x2, double y2);

#endif
