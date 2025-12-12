#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "gammonutil.c"

const char STARTING_BOARD[] = {0,0,0,0,1,14};

// chance of winning = (chance winning turn 1) + (chance winning turn2 - op winning turn 1) 

int runGame(const char* startingBoard, size_t size) {
    Board player1Board = { .size = size };
    memcpy(player1Board.tiles,startingBoard,size * sizeof(char));

    int moves = 0;
    while (true) {
        moves++;
        if (player_turn(&player1Board)) return moves;
    }
}

double runSimulation(const char* startingBoard, float* winChances, uint64_t trials) {
    uint64_t totalMoves = 0;
    uint32_t winsPerTurn[22] = {};
    size_t size = determineSize(startingBoard);
    for (uint64_t i = 0; i < trials; i++) {
        uint8_t moves = runGame(startingBoard, size);
        totalMoves += moves;
        winsPerTurn[(moves > 21) ? 21 : moves]++;
    }
    for(int i = 0; i < 22; i++) {
        winChances[i] = winsPerTurn[i] / (float)trials;
    }
    return totalMoves / (double) trials;
}

uint32_t compressBoard(const char* board) {
    uint32_t result = 0;
    for (int i = 0; i < 6; i++) {
        result |= board[i] << (i * 4);
    }
    return result;
}

double winChance(float* heroChances, float* oppChances) {
    float winChance = 0.0;
    float oppWinChance = 0.0;
    for (int i = 0; i < 25; i++) {
        winChance += heroChances[i] * (1 - oppWinChance);
        oppWinChance += oppChances[i];
    }
    return winChance;
}

int main() {
    initRandom();
    uint64_t trials = 10000000;
    float winsPerTurnBuf[22] = {};
    double averageMoves = runSimulation(STARTING_BOARD, winsPerTurnBuf, trials);
    for (int i = 0; i < 22; i++) {

    }
    return 0;
}
