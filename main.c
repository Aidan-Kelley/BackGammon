#include <stdio.h>
#include "random.c"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "gammonutil.c"

#define PLAYER_ONE_BOARD {1,0,2,1,4,6}
#define PLAYER_TWO_BOARD {0,3,0,0,3,8}

bool runGame() {
    Board player1Board = {0, PLAYER_ONE_BOARD};
    Board player2Board = {0, PLAYER_TWO_BOARD};

    determineSize(&player1Board);
    determineSize(&player2Board);

    while (true) {
        if (player_turn(&player1Board)) return true;
        if (player_turn(&player2Board)) return false;
    }
}
bool runDebugGame() {
    Board player1Board = {0, PLAYER_ONE_BOARD};
    Board player2Board = {0, PLAYER_TWO_BOARD};

    determineSize(&player1Board);
    determineSize(&player2Board);

    while (true) { 
        int* currentRoll = roll(); 
        if (makeMove(&player1Board, currentRoll[0])) return true; 
        if (makeMove(&player1Board, currentRoll[1])) return true; 
        if(currentRoll[0] == currentRoll[1]) { // doubles 
            if (makeMove(&player1Board, currentRoll[0])) return true; 
            if (makeMove(&player1Board, currentRoll[1])) return true; 
        }
        print(player1Board,currentRoll, 1); 
        
        currentRoll = roll();

        if (makeMove(&player2Board, currentRoll[0])) return false; 
        if (makeMove(&player2Board, currentRoll[1])) return false; 
        if(currentRoll[0] == currentRoll[1]) { // doubles 
            if (makeMove(&player2Board, currentRoll[0])) return false; 
            if (makeMove(&player2Board, currentRoll[1])) return false; 
        } 
        print(player2Board,currentRoll, 2); 
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
