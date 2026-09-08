#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/common.h"
#include "../include/battlefield.h"

int main(void) {
    srand((unsigned int)time(NULL));
    EscortSpec specs[5];
    initEscortSpecs(specs);

    Battleship b;
    initBattleship(&b, 'U', 500, 500, 150);
    printBattleship(&b);

    Escortship escorts[5];
    for (int i = 0; i < 5; i++) {
        generateEscortShip(&escorts[i], i, 1000, 150, specs);
        printEscortShip(&escorts[i], specs);
    }
    return 0;
}
