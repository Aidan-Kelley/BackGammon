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

float runSimulation(const char* startingBoard, uint64_t trials) {
    uint64_t moves = 0;
    size_t size = determineSize(startingBoard);
    for (uint64_t i = 0; i < trials; i++) {
        moves += runGame(startingBoard, size);
    }
    return moves / (float) trials;
}

uint32_t compressBoard(const char* board) {
    uint32_t result = 0;
    for (int i = 0; i < 6; i++) {
        result |= board[i] << (i * 4);
    }
    return result;
}

int main() {
    // char b[6] = {3,2,0,0,6,3};
    // float averageMoves = runSimulation(b,10000000);
    // printf("Average of %f moves\n",averageMoves);
    // return 0;
    initRandom();
    const uint64_t trials = 100000;
    float averageMoves;
    int x0, x1, x2, x3, x4, x5, x6;
    float* table = malloc(15728641 * sizeof(float));
    for (x0 = 0; x0 <= 15; x0++) {
        printf("outermost loop%d\n",x0);
        for (x1 = 0; x1 <= 15 - x0; x1++) {
            printf("middle loop%d\n",x1);
            for (x2 = 0; x2 <= 15 - x0 - x1; x2++) {
                printf("inner loop%d",x2);
                for (x3 = 0; x3 <= 15 - x0 - x1 - x2; x3++) {
                    for (x4 = 0; x4 <= 15 - x0 - x1 - x2 - x3; x4++) {
                        for (x5 = 0; x5 <= 15 - x0 - x1 - x2 - x3 - x4; x5++) {

                            x6 = 15 - (x0 + x1 + x2 + x3 + x4 + x5);
                            // printf("(%d, %d, %d, %d, %d, %d)\n",
                            //        x1, x2, x3, x4, x5, x6);
                            char state[6] = {x1, x2, x3, x4, x5, x6};
                            averageMoves = runSimulation(state,trials);
                            table[compressBoard(state)] = averageMoves;
                        }
                    }
                }
            }
        }
    }
    FILE *f = fopen("table.bin", "wb");
    fwrite(table, sizeof(float), 15728640, f);
    fclose(f);
    free(table);
    return 0;
}
