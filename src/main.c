#include <stdio.h>
#include "../include/physics.h"

/* This is a TEMPORARY test harness for Step 1.
   Once we start building the real simulator (Part 1-A logic),
   this will be replaced by the actual menu system. For now it just
   proves the physics functions behave correctly. */

int main(void) {
    double speeds[] = {50, 100, 150};
    double angles[]  = {15, 30, 45, 60, 75};

    printf("Testing projectile motion functions\n");
    printf("(g = 9.8 m/s^2)\n\n");
    printf("%-10s %-10s %-15s %-15s\n", "Speed", "Angle", "Range (m)", "Time (s)");
    printf("--------------------------------------------------\n");

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 5; j++) {
            double u = speeds[i];
            double theta = angles[j];
            double r = calcRange(u, theta);
            double t = timeOfFlight(u, theta);
            printf("%-10.1f %-10.1f %-15.2f %-15.2f\n", u, theta, r, t);
        }
    }

    /* Sanity check: range should be maximum at 45 degrees for a fixed speed */
    printf("\nCheck: for u=100, range at 45 deg should be the largest in that row.\n");

    /* Sanity check: distanceBetween */
    double d = distanceBetween(0, 0, 3, 4);
    printf("\nCheck: distance from (0,0) to (3,4) should be 5.00 -> got %.2f\n", d);

    return 0;
}
