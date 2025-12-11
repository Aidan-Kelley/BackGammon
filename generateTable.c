#include <stdio.h>
#include "random.c"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
    char size;
    char tiles[6];
} Board;

const char STARTING_BOARD[] = {0,0,3,3,4,4};

static inline bool makeMove(Board* board, int die) {
    char* tiles = board->tiles;
    char size = board->size;
    if (die >= size) {
        tiles[size - 1]--;
    } else if (tiles[die] != 0) {
        tiles[die]--;
    } else {
        tiles[size - 1]--;
        tiles[size - 1 - (die + 1)]++;
    }
    while (tiles[board->size - 1] == 0) {
        board->size--;
        if(board->size == 0)
            return true;
    }
    return false;
}

int* roll() {
    int die1 = randInt6();
    int die2 = randInt6();
    static int arr[2];
    if (die1 > die2) {
        arr[0] = die1;
        arr[1] = die2;
    } else {
        arr[0] = die2;
        arr[1] = die1;
    }
    return arr;
}

void print(const Board board, const int* roll, int p) {
    printf("Player %d Roll: %d, %d\n", p, roll[0], roll[1]);
    printf("Player %d Board: ", p);
    printf("%d",board.tiles[0]);
    for (int i = 1; i < board.size; i++) {
        printf(", %d",board.tiles[i]);
    }
    printf("\n");
}

size_t determineSize(const char* board) {
    for (int i = 5; i >= 0; i--) {
        if (board[i] != 0) {
            return i + 1;
        }
    }
}

bool player_turn(Board* board) {
    int* currentRoll = roll(); 
    if (makeMove(board, currentRoll[0])) return true;
    if (makeMove(board, currentRoll[1])) return true;
    if(currentRoll[0] == currentRoll[1]) { // doubles
        if (makeMove(board, currentRoll[0])) return true;
        if (makeMove(board, currentRoll[1])) return true;
    }
    return false;
}

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
    // initRandom();
    // char b[6] = {3,2,0,0,6,3};
    // float averageMoves = runSimulation(b,10000000);
    // printf("Average of %f moves\n",averageMoves);
    // return 0;
    const uint64_t trials = 1000000;
    double averageMoves;
    int x1, x2, x3, x4, x5, x6;
    int count = 0;
    float* table = malloc(15728641 * sizeof(float));
    int fifteen = 15;
    for (x1 = 0; x1 <= fifteen; x1++) {
        printf("outermost loop%d",x1);
        for (x2 = 0; x2 <= fifteen - x1; x2++) {
            printf("inner loop%d",x2);
            for (x3 = 0; x3 <= fifteen - x1 - x2; x3++) {
                for (x4 = 0; x4 <= fifteen - x1 - x2 - x3; x4++) {
                    for (x5 = 0; x5 <= 15 - x1 - x2 - x3 - x4; x5++) {

                        x6 = 15 - (x1 + x2 + x3 + x4 + x5);
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
    FILE *f = fopen("table.bin", "wb");
    fwrite(table, sizeof(float), 15728640, f);
    fclose(f);
    free(table);
    printf("%d",count);
    return 0;
}
