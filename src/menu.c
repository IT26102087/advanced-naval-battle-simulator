#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <dirent.h>
#include "../include/common.h"
#include "../include/physics.h"
#include "../include/battlefield.h"
#include "../include/attack.h"
#include "../include/simulation.h"
#include "../include/simulation1b.h"
#include "../include/fileio.h"
#include "../include/simulation2a.h"
#include "../include/simulation2b.h"
#include "../include/menu.h"

static void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

static void setupBattlefield(SimSettings *s, Battleship *b, Escortship escorts[], EscortSpec specs[5]) {
    if (s->seed != 0) srand(s->seed); else srand((unsigned int)time(NULL));
    initEscortSpecs(specs);
    initBattleship(b, s->bNotation, s->bX, s->bY, s->bVMax);
    for (int i = 0; i < s->N; i++) generateEscortShip(&escorts[i], i, s->D, s->bVMax, specs);
}

static void printBattlefield(Battleship *b, Escortship escorts[], int N, EscortSpec specs[5]) {
    printBattleship(b);
    for (int i = 0; i < N; i++) printEscortShip(&escorts[i], specs);
}

static void doSetup(SimSettings *s) {
    int choice = -1;
    while (choice != 4) {
        printf("\n----- Setup Menu -----\n");
        printf("1. Battleship Properties\n2. Escort/Canvas Settings\n3. Seed value\n4. Return\nChoice: ");
        int r = scanf("%d", &choice);
        if (r == EOF) { printf("\nInput ended.\n"); return; }
        if (r != 1) { clearInputBuffer(); continue; }
        clearInputBuffer();
        if (choice == 1) {
            printf("Battleship type (U/M/R/S): ");
            s->bNotation = getchar(); clearInputBuffer();
            printf("Max shell speed (0 = random): ");
            scanf("%lf", &s->bVMax); clearInputBuffer();
            if (s->bVMax <= 0) s->bVMax = 100 + (rand() % 100);
            printf("Start X,Y (-1 -1 = canvas center): ");
            scanf("%lf %lf", &s->bX, &s->bY); clearInputBuffer();
            if (s->bX < 0 || s->bY < 0) { s->bX = s->D / 2; s->bY = s->D / 2; }
        } else if (choice == 2) {
            printf("Canvas size D: "); scanf("%lf", &s->D); clearInputBuffer();
            printf("Number of escort ships N (max %d): ", MAX_ESCORTS);
            scanf("%d", &s->N); clearInputBuffer();
            if (s->N > MAX_ESCORTS) s->N = MAX_ESCORTS;
        } else if (choice == 3) {
            printf("Seed (0 = current time): "); scanf("%u", &s->seed); clearInputBuffer();
        }
    }
}

static void doPart1A(SimSettings *s) {
    EscortSpec specs[5]; Battleship b; Escortship escorts[MAX_ESCORTS];
    setupBattlefield(s, &b, escorts, specs);
    forceOneHitKillMode(escorts, s->N);

    printBattlefield(&b, escorts, s->N, specs);
    saveInitialConditions("p1a_initial.txt", s->D, &b, escorts, s->N, specs);

    BattleResult result;
    runBattle(&b, escorts, s->N, &result);

    if (result.battleshipSunk)
        printf("\nBattleship SUNK by escort %d.\n", result.sunkByEscortId);
    else
        printf("\nBattleship survived. Destroyed %d escorts in %.2f s.\n",
               result.numHits, result.battleDuration);

    saveBattleResult("p1a_result.txt", &result);
    saveFinalConditions("p1a_result.txt", &b, escorts, s->N);
    printf("\nPress Enter to continue..."); getchar();
}

static void doPart1B(SimSettings *s) {
    EscortSpec specs[5]; Battleship b; Escortship escorts[MAX_ESCORTS];
    setupBattlefield(s, &b, escorts, specs);
    forceOneHitKillMode(escorts, s->N);
    printBattlefield(&b, escorts, s->N, specs);
    saveInitialConditions("p1b_initial.txt", s->D, &b, escorts, s->N, specs);

    printf("How many points k should B visit? ");
    int k; scanf("%d", &k); clearInputBuffer();

    printf("Run with gun jam feature (Simulation 2)? (y/n): ");
    char c = getchar(); clearInputBuffer();

    if (c == 'y' || c == 'Y') {
        printf("Jam after how many iterations (t < %d)? ", k);
        int t; scanf("%d", &t); clearInputBuffer();
        printf("Enter thetaMin (0-30): ");
        double thetaMin; scanf("%lf", &thetaMin); clearInputBuffer();
        runPart1BSimulation2(&b, escorts, s->N, s->D, k, t, thetaMin);
    } else {
        runPart1BSimulation1(&b, escorts, s->N, s->D, k);
    }
    printf("\nPress Enter to continue..."); getchar();
}

static void doPart1C(SimSettings *s) {
    EscortSpec specs[5]; Battleship b; Escortship escorts[MAX_ESCORTS];
    setupBattlefield(s, &b, escorts, specs);
    printBattlefield(&b, escorts, s->N, specs);
    saveInitialConditions("p1c_initial.txt", s->D, &b, escorts, s->N, specs);

    BattleResult result;
    runBattle(&b, escorts, s->N, &result);

    if (result.battleshipSunk)
        printf("\nBattleship SUNK. Contributing escorts: %d, final blow by %d.\n",
               result.numContributors, result.sunkByEscortId);
    else
        printf("\nBattleship survived with %.1f%% cumulative damage. Destroyed %d escorts.\n",
               result.finalDamageOnB * 100, result.numHits);

    saveBattleResult("p1c_result.txt", &result);
    saveFinalConditions("p1c_result.txt", &b, escorts, s->N);
    printf("\nPress Enter to continue..."); getchar();
}

static void doPart2A(SimSettings *s) {
    EscortSpec specs[5]; Battleship b; Escortship escorts[MAX_ESCORTS];
    setupBattlefield(s, &b, escorts, specs);
    printBattlefield(&b, escorts, s->N, specs);
    saveInitialConditions("p2a_initial.txt", s->D, &b, escorts, s->N, specs);

    printf("Enter TB (reload time in seconds) for the battleship: ");
    double TB; scanf("%lf", &TB); clearInputBuffer();

    BattleResult result;
    runBattleWithReload(&b, escorts, s->N, TB, &result);

    if (result.battleshipSunk)
        printf("\nBattleship SUNK. Contributing escorts: %d, final blow by %d.\n",
               result.numContributors, result.sunkByEscortId);
    else
        printf("\nBattleship survived with %.1f%% cumulative damage. Destroyed %d escorts in %.2f s.\n",
               result.finalDamageOnB * 100, result.numHits, result.battleDuration);

    saveBattleResult("p2a_result.txt", &result);
    saveFinalConditions("p2a_result.txt", &b, escorts, s->N);
    printf("\nPress Enter to continue..."); getchar();
}
static void doPart2BC(SimSettings *s) {
    EscortSpec specs[5]; Battleship b; Escortship escorts[MAX_ESCORTS];
    setupBattlefield(s, &b, escorts, specs);
    printBattlefield(&b, escorts, s->N, specs);
    saveInitialConditions("p2bc_initial.txt", s->D, &b, escorts, s->N, specs);

    printf("Enter TB (battleship reload time): ");
    double TB; scanf("%lf", &TB); clearInputBuffer();

    double TE[5];
    char *typeNames[5] = {"A", "B", "C", "D", "E"};
    for (int i = 0; i < 5; i++) {
        printf("Enter TE for escort type %s: ", typeNames[i]);
        scanf("%lf", &TE[i]);
    }
    clearInputBuffer();

    printf("Apply impact power degradation (Part 2-C)? (y/n): ");
    char c = getchar(); clearInputBuffer();
    int useGamma = (c == 'y' || c == 'Y');

    BattleResult result;
    runAdvancedBattle(&b, escorts, s->N, TB, TE, useGamma, &result);

    if (result.battleshipSunk)
        printf("\nBattleship SUNK. Contributing hits: %d, final blow by escort %d.\n",
               result.numContributors, result.sunkByEscortId);
    else {
        int destroyedCount = 0;
        for (int i = 0; i < s->N; i++) if (!escorts[i].alive) destroyedCount++;

        printf("\nBattleship survived. Damage taken: %.1f%%. Escorts destroyed: %d / %d. Total shots fired by B: %d. Duration: %.2f s.\n",
               result.finalDamageOnB * 100, destroyedCount, s->N, result.numHits, result.battleDuration);
    } 

    saveBattleResult("p2bc_result.txt", &result);
    saveFinalConditions("p2bc_result.txt", &b, escorts, s->N);
    printf("\nPress Enter to continue..."); getchar();
}
static void showInstructions(void) {
    printf("\n----- Instructions -----\n");
    printf("Stationary Battleship (B) vs many stationary Escort ships (E).\n");
    printf("Part 1-A: single hit destroys either ship.\n");
    printf("Part 1-B: B moves to k random points, battle repeats at each.\n");
    printf("Part 1-C: E ships deal partial cumulative damage to B.\n");
    printf("Configure via Setup, then pick a Part to run. Results save to .txt files.\n");
    printf("\nPress Enter to return..."); getchar();
}

static void showStatistics(void) {
    printf("\n----- Simulation Statistics -----\nSaved files:\n\n");
    DIR *d = opendir(".");
    if (!d) { printf("Cannot open directory.\n"); return; }
    struct dirent *entry; int count = 0;
    while ((entry = readdir(d)) != NULL) {
        char *name = entry->d_name; int len = strlen(name);
        if (len > 4 && strcmp(name + len - 4, ".txt") == 0) printf("  %d. %s\n", ++count, name);
    }
    closedir(d);
    if (count == 0) { printf("  (none yet)\n\nPress Enter..."); getchar(); return; }
    printf("\nEnter filename to view (or press Enter to skip): ");
    char filename[100]; fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = 0;
    if (strlen(filename) > 0) {
        FILE *fp = fopen(filename, "r");
        if (!fp) printf("Could not open %s\n", filename);
        else { char line[256]; while (fgets(line, sizeof(line), fp)) printf("%s", line); fclose(fp); }
    }
    printf("\nPress Enter to return..."); getchar();
}

void runMainMenu(void) {
    SimSettings s = {1000, 5, 'U', 150, 500, 500, 0};
    int choice = -1;
    while (choice != 5) {
        printf("\n===== Advanced Naval Battle Simulator =====\n");
        printf("1. Setup\n2. Run Part 1-A (single battle)\n3. Run Part 1-B (moving battleship)\n");
        printf("4. Run Part 1-C (cumulative damage)\n5. Exit\n6. Instructions\n7. Statistics\n8. Run Part 2-A (reload + strategy) \n9. Run Part 2-B/C (repeated fire + decay)\nChoice: ");
        int r = scanf("%d", &choice);
        if (r == EOF) { printf("\nInput ended.\n"); return; }
        if (r != 1) { clearInputBuffer(); continue; }
        clearInputBuffer();
        switch (choice) {
            case 1: doSetup(&s); break;
            case 2: doPart1A(&s); break;
            case 3: doPart1B(&s); break;
            case 4: doPart1C(&s); break;
            case 5: printf("Goodbye!\n"); break;
            case 6: showInstructions(); choice = -1; break;
            case 7: showStatistics(); choice = -1; break;
	    case 8: doPart2A(&s); choice = -1; break;
	    case 9: doPart2BC (&s); choice = -1; break;
            default: printf("Invalid choice.\n");
        }
    }
}
