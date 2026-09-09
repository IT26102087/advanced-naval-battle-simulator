#include <stdio.h>
#include "../include/common.h"
#include "../include/fileio.h"

void saveInitialConditions(const char *filename, double D, Battleship *b,
                            Escortship escorts[], int N, EscortSpec specs[5]) {
    FILE *fp = fopen(filename, "w");
    if (!fp) { printf("Error opening %s\n", filename); return; }

    fprintf(fp, "===== INITIAL BATTLEFIELD CONDITIONS =====\n");
    fprintf(fp, "Canvas: (0,0) to (%.2f,%.2f)\n\n", D, D);

    fprintf(fp, "--- Battleship ---\n");
    fprintf(fp, "Type: %c (%s)\n", b->notation, b->name);
    fprintf(fp, "Position: (%.2f, %.2f)\n", b->x, b->y);
    fprintf(fp, "Velocity range: %.2f to %.2f\n", b->vMin, b->vMax);
    fprintf(fp, "Impact power: %.3f\n", b->impactPower);
    fprintf(fp, "Gamma: %.4f\n\n", b->gamma);

    fprintf(fp, "--- Escort Ships (%d) ---\n", N);
    for (int i = 0; i < N; i++) {
        Escortship *e = &escorts[i];
        fprintf(fp, "\nEscort #%d [Type %c - %s]\n", e->id, 'A' + e->type, specs[e->type].typeName);
        fprintf(fp, "  Position: (%.2f, %.2f)\n", e->x, e->y);
        fprintf(fp, "  Velocity range: %.2f to %.2f\n", e->vMin, e->vMax);
        fprintf(fp, "  Angle range: %.2f to %.2f\n", e->angleMin, e->angleMax);
        fprintf(fp, "  Impact power: %.3f\n", e->impactPower);
        fprintf(fp, "  Gamma: %.4f\n", e->gamma);
    }
    fclose(fp);
    printf("Saved: %s\n", filename);
}

void saveBattleResult(const char *filename, BattleResult *result) {
    FILE *fp = fopen(filename, "w");
    if (!fp) { printf("Error opening %s\n", filename); return; }

    fprintf(fp, "===== BATTLE RESULT =====\n\n");
    if (result->battleshipSunk) {
        fprintf(fp, "Battleship: SUNK\n");
        fprintf(fp, "Final blow by Escort ID: %d\n", result->sunkByEscortId);
        fprintf(fp, "Total contributing escort hits: %d\n", result->numContributors);
        fprintf(fp, "Contributor escort IDs: ");
        for (int i = 0; i < result->numContributors; i++) fprintf(fp, "%d ", result->contributors[i]);
        fprintf(fp, "\n");
    } else {
        fprintf(fp, "Battleship: SURVIVED\n");
        fprintf(fp, "Cumulative damage taken: %.3f (of 1.0)\n", result->finalDamageOnB);
        fprintf(fp, "Escort ships destroyed by B: %d\n", result->numHits);
        fprintf(fp, "Battle duration: %.2f s\n\n", result->battleDuration);
        for (int i = 0; i < result->numHits; i++) {
            fprintf(fp, "  Escort %d | range %.2f | time to hit %.2f s\n",
                    result->hits[i].escortId, result->hits[i].range, result->hits[i].timeToHit);
        }
    }
    fclose(fp);
    printf("Saved: %s\n", filename);
}

void saveFinalConditions(const char *filename, Battleship *b, Escortship escorts[], int N) {
    FILE *fp = fopen(filename, "a");
    if (!fp) { printf("Error opening %s\n", filename); return; }

    fprintf(fp, "\n===== FINAL CONDITIONS =====\n");
    fprintf(fp, "Battleship alive: %s | damage taken: %.3f\n",
            b->alive ? "yes" : "no", b->damageTaken);

    int aliveCount = 0;
    for (int i = 0; i < N; i++) if (escorts[i].alive) aliveCount++;
    fprintf(fp, "Escort ships alive: %d / %d\n", aliveCount, N);
    for (int i = 0; i < N; i++) {
        fprintf(fp, "  Escort #%d: %s\n", escorts[i].id, escorts[i].alive ? "alive" : "destroyed");
    }
    fclose(fp);
    printf("Appended: %s\n", filename);
}
