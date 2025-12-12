#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "gammonutil.c"

#define PLAYER_ONE_BOARD {0,3,1,0,2,1}
#define PLAYER_TWO_BOARD {1,1,2,3,3,3}

bool runGame() {
    Board player1Board = {0, PLAYER_ONE_BOARD};
    Board player2Board = {0, PLAYER_TWO_BOARD};

    player1Board.size = determineSize((const char*)&player1Board.tiles);
    player2Board.size = determineSize((const char*)&player2Board.tiles);

    while (true) {
        if (player_turn(&player1Board)) return true;
        if (player_turn(&player2Board)) return false;
    }
}

int main() {

    initRandom();
    clock_t begin, end;
    begin = clock();
    const uint64_t trials = 1000000;
    uint64_t wins = 0;

    for (uint64_t i = 0; i < trials; i++) {
        if (runGame())
            wins++;
    }
    end = clock();
    printf("%.17f in %f\n", wins / (double)trials, (double)(end - begin) / CLOCKS_PER_SEC);
    return 0;
}
