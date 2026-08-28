#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/common.h"
#include "../include/physics.h"
#include "../include/battlefield.h"

/* This is still a TEMPORARY test harness for Step 2.
   It proves battlefield generation works before we build the
   real menu system and battle logic in Step 3+.               */

int main(void) {
    /* Seed the random number generator using the current time,
       so we get different random ships each run. Later, the
       "Seed value" menu option will let the user override this
       with a fixed number (useful for repeatable tests).       */
    srand((unsigned int)time(NULL));

    EscortSpec specs[5];
    initEscortSpecs(specs);

    /* --- Test inputs (later these come from the user via menu) --- */
    double D = 1000;        /* canvas is 1000 x 1000 */
    int N = 5;               /* 5 escort ships */
    double vMaxB = 200;      /* battleship's max shell speed */

    Battleship b;
    initBattleship(&b, 'U', D / 2, D / 2, vMaxB); /* place B in the center */

    EscortShip escorts[MAX_ESCORTS];
    for (int i = 0; i < N; i++) {
        generateEscortShip(&escorts[i], i, D, vMaxB, specs);
    }

    printf("=== Battlefield Setup ===\n");
    printf("Canvas size: %.0f x %.0f\n\n", D, D);

    printBattleship(&b);

    printf("=== Escort Ships (%d) ===\n\n", N);
    for (int i = 0; i < N; i++) {
        printEscortShip(&escorts[i], specs);
    }

    return 0;
}
