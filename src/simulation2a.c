#include <math.h>
#include <stdio.h>
#include "../include/common.h"
#include "../include/simulation2a.h"
#include "../include/attack.h"
#include "../include/physics.h"

void runBattleWithReload(Battleship *b, Escortship escorts[], int N, double TB, BattleResult *result) {
    result->battleshipSunk = 0;
    result->sunkByEscortId = -1;
    result->numContributors = 0;
    result->numHits = 0;
    result->battleDuration = 0.0;
    result->finalDamageOnB = 0.0;

    /* Phase 1: escorts in range fire at B immediately (unchanged from
       Part 1-A/C - only B's firing gets a reload delay in Part 2-A). */
    for (int i = 0; i < N; i++) {
        Escortship *e = &escorts[i];
        if (e->alive && !e->hasFired && isInEscortRange(e, b)) {
            e->hasFired = 1;
            b->damageTaken += e->impactPower;
            result->contributors[result->numContributors] = e->id;
            result->numContributors++;
            if (b->damageTaken >= 0.999999) {
                result->battleshipSunk = 1;
                result->sunkByEscortId = e->id;
                b->alive = 0;
                result->finalDamageOnB = b->damageTaken;
                return;
            }
        }
    }
    result->finalDamageOnB = b->damageTaken;

    /* Phase 2: B fires with reload delay TB between shots.
       Strategy: attack the CLOSEST escort ship first each time - this
       minimizes the time any single nearby threat remains alive.
       Build a list of alive, in-range escort indices, then repeatedly
       pick the nearest remaining one, "fire" (destroy it), advance
       the clock by TB before the next shot.                          */
    int candidates[MAX_ESCORTS];
    int numCandidates = 0;
    for (int i = 0; i < N; i++) {
        if (escorts[i].alive && isInBattleshipRange(b, &escorts[i])) {
            candidates[numCandidates++] = i;
        }
    }

    double clock = 0.0;
    printf("\nAttack order (closest-first strategy):\n");
    for (int shot = 0; shot < numCandidates; shot++) {
        /* find the closest remaining candidate */
        int bestIdx = -1;
        double bestDist = -1;
        for (int c = 0; c < numCandidates; c++) {
            int idx = candidates[c];
            if (!escorts[idx].alive) continue; /* already destroyed */
            double d = distanceBetween(b->x, b->y, escorts[idx].x, escorts[idx].y);
            if (bestIdx == -1 || d < bestDist) {
                bestDist = d;
                bestIdx = idx;
            }
        }
        if (bestIdx == -1) break;

        double speedNeeded = sqrt(bestDist * G);
        double flightTime = timeOfFlight(speedNeeded, 45.0);
        double totalTime = clock + flightTime;

        printf("  Shot %d: Escort %d at range %.2f, fires at t=%.2f, hits at t=%.2f\n",
               shot + 1, escorts[bestIdx].id, bestDist, clock, totalTime);

        HitRecord h;
        h.escortId = escorts[bestIdx].id;
        h.range = bestDist;
        h.timeToHit = totalTime;
        result->hits[result->numHits] = h;
        result->numHits++;

        escorts[bestIdx].alive = 0;
        b->shotsFired++;

        if (totalTime > result->battleDuration) result->battleDuration = totalTime;
        clock += TB; /* B must reload before its next shot */
    }
}
