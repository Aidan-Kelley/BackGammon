#include <stdio.h>
#include "random.c"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
    char size;
    char tiles[6];
} Board;

#define PLAYER_ONE_BOARD {0,3,3,3,3,3}
#define PLAYER_TWO_BOARD {0,0,0,0,0,8}

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
    for (int i = 1; i < 6; i++) {
        printf(", %d",board.tiles[i]);
    }
    printf("\n");
}

void shrinkBoard(Board* board) {
    for (int i = 5; i >= 0; i--) {
        if (board->tiles[i] != 0) {
            board->size = i + 1;
            return;
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

bool runGame() {
    Board player1Board = {0, PLAYER_ONE_BOARD};
    Board player2Board = {0, PLAYER_TWO_BOARD};

    shrinkBoard(&player1Board);
    shrinkBoard(&player2Board);

    while (true) {
        if (player_turn(&player1Board)) return true;
        if (player_turn(&player2Board)) return false;
    }
}
bool runDebugGame() {
    Board player1Board = {0, PLAYER_ONE_BOARD};
    Board player2Board = {0, PLAYER_TWO_BOARD};

    shrinkBoard(&player1Board);
    shrinkBoard(&player2Board);

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
    printf("%.17f in %f\n",wins / (double)trials, (double)(end - begin) / CLOCKS_PER_SEC);
    return 0;
}
