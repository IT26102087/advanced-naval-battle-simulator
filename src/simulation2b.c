#include <math.h>
#include <stdio.h>
#include "../include/common.h"
#include "../include/simulation2b.h"
#include "../include/attack.h"
#include "../include/physics.h"

void runAdvancedBattle(Battleship *b, Escortship escorts[], int N,
                        double TB, double TE[5], int useGammaDecay,
                        BattleResult *result) {
    result->battleshipSunk = 0;
    result->sunkByEscortId = -1;
    result->numContributors = 0;
    result->numHits = 0;
    result->battleDuration = 0.0;
    result->finalDamageOnB = 0.0;

    /* Which escorts can attack B (fixed, since nothing moves), and
       which escorts B can attack - determined once at the start,
       since positions never change during the battle.               */
    int attackers[MAX_ESCORTS], numAttackers = 0;
    int targets[MAX_ESCORTS], numTargets = 0;
    for (int i = 0; i < N; i++) {
        if (isInEscortRange(&escorts[i], b)) attackers[numAttackers++] = i;
        if (isInBattleshipRange(b, &escorts[i])) targets[numTargets++] = i;
    }

    /* Per-entity firing state: next time they're allowed to fire,
       and how many shots they've fired so far (needed for the
       gamma decay formula IPn = IP0 * e^(-gamma*n)).                 */
    double nextFireE[MAX_ESCORTS];
    int shotsFiredE[MAX_ESCORTS];
    for (int i = 0; i < numAttackers; i++) {
        nextFireE[i] = 0.0;
        shotsFiredE[i] = 0;
    }
    double nextFireB = 0.0;
    int shotsFiredB = 0;

    int maxIterations = 200; /* safety cap: with decay, damage can
                                 converge and never reach the kill
                                 threshold, so we must not loop forever */

    for (int iter = 0; iter < maxIterations; iter++) {
        if (!b->alive) break;

        /* does B still have an alive target? */
        int bHasTarget = 0;
        for (int t = 0; t < numTargets; t++) if (escorts[targets[t]].alive) bHasTarget = 1;

        /* find the next event: whichever of (B, each attacker) has
           the smallest nextFireTime and is still eligible to fire.   */
        double bestTime = -1;
        int isB = 0, attackerIdx = -1;

        if (bHasTarget) { bestTime = nextFireB; isB = 1; }
        for (int i = 0; i < numAttackers; i++) {
            if (!escorts[attackers[i]].alive) continue;
            if (bestTime < 0 || nextFireE[i] < bestTime) {
                bestTime = nextFireE[i]; isB = 0; attackerIdx = i;
            }
        }
        if (bestTime < 0) break; /* nobody left who can fire */

        if (isB) {
            /* B fires at the closest alive target */
            int bestT = -1; double bestDist = -1;
            for (int t = 0; t < numTargets; t++) {
                int idx = targets[t];
                if (!escorts[idx].alive) continue;
                double d = distanceBetween(b->x, b->y, escorts[idx].x, escorts[idx].y);
                if (bestT == -1 || d < bestDist) { bestDist = d; bestT = idx; }
            }
            double impact = useGammaDecay
                ? b->impactPower * exp(-b->gamma * shotsFiredB)
                : 1.0; /* without decay, B always one-shots per spec */
            escorts[bestT].damageTaken += impact;
            shotsFiredB++;

            if (result->numHits < MAX_ESCORTS) {
                HitRecord h = { escorts[bestT].id, bestDist, bestTime };
                result->hits[result->numHits++] = h;
            }
            if (escorts[bestT].damageTaken >= 0.999999) {
                escorts[bestT].alive = 0;
            }
            nextFireB = bestTime + TB;
            if (bestTime > result->battleDuration) result->battleDuration = bestTime;

        } else {
            /* this attacker fires at B */
            Escortship *e = &escorts[attackers[attackerIdx]];
            double impact = useGammaDecay
                ? e->impactPower * exp(-e->gamma * shotsFiredE[attackerIdx])
                : e->impactPower;
            b->damageTaken += impact;
            shotsFiredE[attackerIdx]++;

            if (result->numContributors < MAX_ESCORTS) {
                result->contributors[result->numContributors++] = e->id;
            }
            if (bestTime > result->battleDuration) result->battleDuration = bestTime;

            if (b->damageTaken >= 0.999999) {
                result->battleshipSunk = 1;
                result->sunkByEscortId = e->id;
                b->alive = 0;
                result->finalDamageOnB = b->damageTaken;
                return;
            }
            nextFireE[attackerIdx] = bestTime + TE[e->type];
        }
    }

    result->finalDamageOnB = b->damageTaken;
}
