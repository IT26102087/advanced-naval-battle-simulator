#ifndef FILEIO_H
#define FILEIO_H
#include "common.h"
#include "simulation.h"

void saveInitialConditions(const char *filename, double D, Battleship *b,
                            Escortship escorts[], int N, EscortSpec specs[5]);
void saveBattleResult(const char *filename, BattleResult *result);
void saveFinalConditions(const char *filename, Battleship *b, Escortship escorts[], int N);

#endif
