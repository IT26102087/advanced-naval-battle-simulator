#ifndef BATTLEFIELD_H
#define BATTLEFIELD_H
#include "common.h"

void initEscortSpecs(EscortSpec specs[5]);
double randDouble(double min, double max);
void generateEscortShip(Escortship *e, int id, double D, double vMaxB, EscortSpec specs[5]);
void initBattleship(Battleship *b, char notation, double x, double y, double vMax);
void printEscortShip(Escortship *e, EscortSpec specs[5]);
void printBattleship(Battleship *b);

#endif
