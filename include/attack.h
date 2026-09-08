#ifndef ATTACK_H
#define ATTACK_H
#include "common.h"

double battleshipMaxRange(Battleship *b);
int isInBattleshipRange(Battleship *b, Escortship *e);
double escortMinRange(Escortship *e);
double escortMaxRange(Escortship *e);
int isInEscortRange(Escortship *e, Battleship *b);

#endif
