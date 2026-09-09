#include <stdio.h>
#include "../include/common.h"
#include "../include/battlefield.h"
#include "../include/simulation.h"
#include "../include/simulation1b.h"
#include "../include/fileio.h"

void runPart1BSimulation1(Battleship *b, Escortship escorts[], int N, double D, int k) {
    printf("\n===== Part 1-B, Simulation 1: Moving Battleship =====\n");
    for (int iter = 1; iter <= k; iter++) {
        if (!b->alive) {
            printf("B already sunk - stopping at iteration %d.\n", iter);
            break;
        }
        b->x = randDouble(0, D);
        b->y = randDouble(0, D);
        printf("\n--- Iteration %d: B moves to (%.2f, %.2f) ---\n", iter, b->x, b->y);

        BattleResult result;
        runBattle(b, escorts, N, &result);

        char filename[64];
        snprintf(filename, sizeof(filename), "part1b_sim1_iter%d.txt", iter);
        saveBattleResult(filename, &result);
        saveFinalConditions(filename, b, escorts, N);

        if (result.battleshipSunk) {
            printf("B SUNK at iteration %d by escort %d.\n", iter, result.sunkByEscortId);
            break;
        }
        printf("Iteration %d: destroyed %d escort(s), duration %.2f s.\n",
               iter, result.numHits, result.battleDuration);
    }
}

void runPart1BSimulation2(Battleship *b, Escortship escorts[], int N, double D,
                           int k, int t, double thetaMin) {
    printf("\n===== Part 1-B, Simulation 2: Moving Battleship + Gun Jam =====\n");
    printf("Gun jams after iteration %d (angle restricted to %.1f-90 deg)\n", t, thetaMin);

    for (int iter = 1; iter <= k; iter++) {
        if (!b->alive) {
            printf("B already sunk - stopping at iteration %d.\n", iter);
            break;
        }
        b->x = randDouble(0, D);
        b->y = randDouble(0, D);
        int jammed = (iter > t);
        printf("\n--- Iteration %d: B moves to (%.2f, %.2f) %s ---\n",
               iter, b->x, b->y, jammed ? "[GUN JAMMED]" : "");

        BattleResult result;
        runBattle(b, escorts, N, &result);

        char filename[64];
        snprintf(filename, sizeof(filename), "part1b_sim2_iter%d.txt", iter);
        saveBattleResult(filename, &result);
        saveFinalConditions(filename, b, escorts, N);

        if (result.battleshipSunk) {
            printf("B SUNK at iteration %d by escort %d.\n", iter, result.sunkByEscortId);
            break;
        }
        printf("Iteration %d: destroyed %d escort(s), duration %.2f s.\n",
               iter, result.numHits, result.battleDuration);
    }
}
