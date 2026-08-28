#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/common.h"
#include "../include/battlefield.h"

void initEscortSpecs(EscortSpec specs[5]) {
    /* Data taken directly from Table 1 in the assignment spec.
       Index into this array using the EscortType enum, e.g.
       specs[TYPE_A] gives you the 1936A-class Destroyer's data. */

    strcpy(specs[TYPE_A].typeName, "1936A-class Destroyer");
    strcpy(specs[TYPE_A].gunName, "SK C/34 naval gun");
    specs[TYPE_A].impactPower = 0.08;
    specs[TYPE_A].angleRange  = 20;

    strcpy(specs[TYPE_B].typeName, "Gabbiano-class Corvette");
    strcpy(specs[TYPE_B].gunName, "L/47 dual-purpose gun");
    specs[TYPE_B].impactPower = 0.06;
    specs[TYPE_B].angleRange  = 30;

    strcpy(specs[TYPE_C].typeName, "Matsu-class Destroyer");
    strcpy(specs[TYPE_C].gunName, "Type 89 dual-purpose gun");
    specs[TYPE_C].impactPower = 0.07;
    specs[TYPE_C].angleRange  = 25;

    strcpy(specs[TYPE_D].typeName, "F-class Escort Ships");
    strcpy(specs[TYPE_D].gunName, "SK C/32 naval gun");
    specs[TYPE_D].impactPower = 0.05;
    specs[TYPE_D].angleRange  = 50;

    strcpy(specs[TYPE_E].typeName, "Japanese Kaibokan");
    strcpy(specs[TYPE_E].gunName, "(4.7 inch) naval guns");
    specs[TYPE_E].impactPower = 0.04;
    specs[TYPE_E].angleRange  = 70;
}

double randDouble(double min, double max) {
    /* rand() gives an int between 0 and RAND_MAX.
       Dividing by RAND_MAX scales it to a fraction between 0 and 1,
       then we stretch that fraction to fit the [min, max] range. */
    double fraction = (double)rand() / (double)RAND_MAX;
    return min + fraction * (max - min);
}

void generateEscortShip(EscortShip *e, int id, double D, double vMaxB,
                         EscortSpec specs[5]) {
    e->id = id;

    /* Pick a random type: rand() % 5 gives 0-4, matching the enum values */
    e->type = (EscortType)(rand() % 5);

    /* Random position anywhere inside the D x D canvas */
    e->x = randDouble(0, D);
    e->y = randDouble(0, D);

    /* --- Velocity rules from Table 1 --- */
    if (e->type == TYPE_A) {
        /* Type A's max velocity is a FIXED formula, not random:
           1.2 * VmaxB */
        e->vMax = 1.2 * vMaxB;
    } else {
        /* All other types: randomly generated, but must stay below VmaxB */
        e->vMax = randDouble(1, vMaxB);
    }
    /* Min velocity: randomly generated, must be less than vMax */
    e->vMin = randDouble(0, e->vMax);

    /* --- Angle rules from Table 1 --- */
    double angleRange = specs[e->type].angleRange;
    /* theta_L (lower bound) is randomly generated, but theta_H
       (= theta_L + angleRange) must not exceed 90 degrees.
       So theta_L can be at most (90 - angleRange).             */
    e->angleMin = randDouble(0, 90 - angleRange);
    e->angleMax = e->angleMin + angleRange;

    /* --- Damage / status --- */
    e->impactPower = specs[e->type].impactPower; /* used from Part 1-C onward */
    e->gamma = 0.0;      /* filled in later, Part 2-C */
    e->alive = 1;         /* starts alive */
    e->hasFired = 0;      /* hasn't fired yet */
}

void initBattleship(Battleship *b, char notation, double x, double y,
                     double vMax) {
    b->notation = notation;

    switch (notation) {
        case 'U': strcpy(b->name, "USS Iowa (BB-61)");      break;
        case 'M': strcpy(b->name, "MS King George V");      break;
        case 'R': strcpy(b->name, "Richelieu");              break;
        case 'S': strcpy(b->name, "Sovetsky Soyuz-class");   break;
        default:  strcpy(b->name, "Unknown");                break;
    }

    b->x = x;
    b->y = y;
    b->vMax = vMax;
    b->vMin = 0;          /* per spec, always 0 */
    b->gamma = 0.0;        /* filled in later, Part 2-C */
    b->impactPower = 1.0;  /* starts at full power (one-hit kill, Parts 1-A/B) */
    b->alive = 1;
}

void printEscortShip(EscortShip *e, EscortSpec specs[5]) {
    printf("Escort #%d [Type %c - %s]\n", e->id, 'A' + e->type,
           specs[e->type].typeName);
    printf("  Position: (%.2f, %.2f)\n", e->x, e->y);
    printf("  Velocity: %.2f to %.2f\n", e->vMin, e->vMax);
    printf("  Angle range: %.2f to %.2f degrees\n", e->angleMin, e->angleMax);
    printf("  Impact power: %.2f | Alive: %s\n\n",
           e->impactPower, e->alive ? "yes" : "no");
}

void printBattleship(Battleship *b) {
    printf("Battleship [%c - %s]\n", b->notation, b->name);
    printf("  Position: (%.2f, %.2f)\n", b->x, b->y);
    printf("  Velocity: %.2f to %.2f\n", b->vMin, b->vMax);
    printf("  Impact power: %.2f | Alive: %s\n\n",
           b->impactPower, b->alive ? "yes" : "no");
}
