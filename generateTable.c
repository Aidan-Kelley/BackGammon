#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "gammonutil.c"

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

int main() {
    initRandom();
    const uint64_t trials = 100;
    float averageMoves;
    int x0, x1, x2, x3, x4, x5, x6;
    FILE *f = fopen("winspercents.bin", "ab");
    for (x0 = 0; x0 <= 14; x0++) {
        printf("outermost loop%d\n",x0);
        for (x1 = 0; x1 <= 15 - x0; x1++) {
            printf("middle loop%d\n",x1);
            for (x2 = 0; x2 <= 15 - x0 - x1; x2++) {
                for (x3 = 0; x3 <= 15 - x0 - x1 - x2; x3++) {
                    for (x4 = 0; x4 <= 15 - x0 - x1 - x2 - x3; x4++) {
                        for (x5 = 0; x5 <= 15 - x0 - x1 - x2 - x3 - x4; x5++) {
                            x6 = 15 - (x0 + x1 + x2 + x3 + x4 + x5);
                            char state[6] = {x1, x2, x3, x4, x5, x6};
                            uint32_t boardId = compressBoard(state);
                            fwrite(&boardId,sizeof(boardId),1,f);
                            float winsPerTurnBuf[22] = {};
                            double averageMoves = runSimulation(state, winsPerTurnBuf, trials);
                            fwrite(winsPerTurnBuf,sizeof(winsPerTurnBuf),1,f);
                        }
                    }
                }
            }
        }
    }
    fclose(f);
    printf("done");
}