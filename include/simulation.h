#ifndef SIMULATION_H
#define SIMULATION_H
#include "common.h"

typedef struct {
    int escortId;
    double range;
    double timeToHit;
} HitRecord;

typedef struct {
    int battleshipSunk;
    int sunkByEscortId;
    int contributors[MAX_ESCORTS];
    int numContributors;
    HitRecord hits[MAX_ESCORTS];
    int numHits;
    double battleDuration;
    double finalDamageOnB;
} BattleResult;

void runBattle(Battleship *b, Escortship escorts[], int N, BattleResult *result);
void forceOneHitKillMode(Escortship escorts[], int N);

#endif
