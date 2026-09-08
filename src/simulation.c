#include <math.h>
#include "../include/common.h"
#include "../include/simulation.h"
#include "../include/attack.h"
#include "../include/physics.h"

void forceOneHitKillMode(Escortship escorts[], int N) {
    for (int i = 0; i < N; i++) {
        escorts[i].impactPower = 1.0;
    }
}

void runBattle(Battleship *b, Escortship escorts[], int N, BattleResult *result) {
    result->battleshipSunk = 0;
    result->sunkByEscortId = -1;
    result->numContributors = 0;
    result->numHits = 0;
    result->battleDuration = 0.0;
    result->finalDamageOnB = 0.0;

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

    double maxFlightTime = 0.0;
    for (int i = 0; i < N; i++) {
        Escortship *e = &escorts[i];
        if (e->alive && isInBattleshipRange(b, e)) {
            double range = distanceBetween(b->x, b->y, e->x, e->y);
            double speedNeeded = sqrt(range * G);
            double flightTime = timeOfFlight(speedNeeded, 45.0);

            HitRecord h;
            h.escortId = e->id;
            h.range = range;
            h.timeToHit = flightTime;
            result->hits[result->numHits] = h;
            result->numHits++;

            e->alive = 0;
            b->shotsFired++;

            if (flightTime > maxFlightTime) maxFlightTime = flightTime;
        }
    }
    result->battleDuration = maxFlightTime;
}
