#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/common.h"
#include "../include/battlefield.h"

void initEscortSpecs(EscortSpec specs[5]) {
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
    double fraction = (double)rand() / (double)RAND_MAX;
    return min + fraction * (max - min);
}

void generateEscortShip(Escortship *e, int id, double D, double vMaxB, EscortSpec specs[5]) {
    e->id = id;
    e->type = rand() % 5;
    e->x = randDouble(0, D);
    e->y = randDouble(0, D);

    if (e->type == TYPE_A) {
        e->vMax = 1.2 * vMaxB;
    } else {
        e->vMax = randDouble(1, vMaxB);
    }
    e->vMin = randDouble(0, e->vMax);

    double angleRange = specs[e->type].angleRange;
    e->angleMin = randDouble(0, 90 - angleRange);
    e->angleMax = e->angleMin + angleRange;

    e->impactPower = specs[e->type].impactPower;
    e->gamma = randDouble(0.05, 0.2);
    e->damageTaken = 0.0;
    e->alive = 1;
    e->hasFired = 0;
}

void initBattleship(Battleship *b, char notation, double x, double y, double vMax) {
    b->notation = notation;
    switch (notation) {
        case 'U': strcpy(b->name, "USS Iowa (BB-61)");    break;
        case 'M': strcpy(b->name, "MS King George V");    break;
        case 'R': strcpy(b->name, "Richelieu");            break;
        case 'S': strcpy(b->name, "Sovetsky Soyuz-class"); break;
        default:  strcpy(b->name, "Unknown");              break;
    }
    b->x = x;
    b->y = y;
    b->vMax = vMax;
    b->vMin = 0;
    b->gamma = randDouble(0.001, 0.02);
    b->impactPower = 1.0;
    b->damageTaken = 0.0;
    b->alive = 1;
    b->shotsFired = 0;
}

void printEscortShip(Escortship *e, EscortSpec specs[5]) {
    printf("Escort #%d [Type %c - %s]\n", e->id, 'A' + e->type, specs[e->type].typeName);
    printf("  Position: (%.2f, %.2f)\n", e->x, e->y);
    printf("  Velocity: %.2f to %.2f\n", e->vMin, e->vMax);
    printf("  Angle range: %.2f to %.2f degrees\n", e->angleMin, e->angleMax);
    printf("  Impact power: %.3f | Alive: %s\n\n", e->impactPower, e->alive ? "yes" : "no");
}

void printBattleship(Battleship *b) {
    printf("Battleship [%c - %s]\n", b->notation, b->name);
    printf("  Position: (%.2f, %.2f)\n", b->x, b->y);
    printf("  Velocity: %.2f to %.2f\n", b->vMin, b->vMax);
    printf("  Impact power: %.2f | Alive: %s\n\n", b->impactPower, b->alive ? "yes" : "no");
}
