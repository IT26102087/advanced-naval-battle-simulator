#ifndef SIMULATION2B_H
#define SIMULATION2B_H
#include "common.h"
#include "simulation.h"

void runAdvancedBattle(Battleship *b, Escortship escorts[], int N,
                        double TB, double TE[5], int useGammaDecay,
                        BattleResult *result);

#endif
