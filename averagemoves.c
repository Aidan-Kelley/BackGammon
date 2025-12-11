#include <stdio.h>
#include "random.c"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
    char size;
    char tiles[6];
} Board;

const char STARTING_BOARD[] = {0,2,3,1,3,1};

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

double runSimulation(const char* startingBoard, uint64_t trials) {
    uint64_t moves = 0;
    size_t size = determineSize(startingBoard);
    for (uint64_t i = 0; i < trials; i++) {
        moves += runGame(startingBoard, size);
    }
    return moves / (double) trials;
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
    uint64_t trials = 10000000;
    double averageMoves = runSimulation(STARTING_BOARD,10000000);
    printf("Average of %f moves\n",averageMoves);
    return 0;
}
