#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

typedef struct {
    uint8_t size;
    uint8_t tiles[6];
} Board;

static uint32_t rng_state = 1u;

static inline void initRandom(void) {
    rng_state = (uint32_t)time(NULL) | 1u;
}

static inline uint32_t fast_rand_u32(void) {
    uint32_t x = rng_state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    rng_state = x;
    return x;
}

static inline int randInt6(void) { // 0..5
    return (int)(fast_rand_u32() % 6u);
}

static inline bool do_turn(Board* board, int die) {
    uint8_t* tiles = board->tiles;
    uint8_t size = board->size;
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

static inline void roll_sorted(int *hi, int *lo) {
    int a = randInt6();   // 0..5
    int b = randInt6();   // 0..5

    // branchless swap
    int bigger = a >= b ? a : b;
    int smaller = a >= b ? b : a;

    *hi = bigger;
    *lo = smaller;
}

static inline bool player_turn(Board *board) {
    int d1, d2;
    roll_sorted(&d1, &d2);
    if (do_turn(board, d1)) return true;
    if (do_turn(board, d2)) return true;

    if (d1 == d2) {
        if (do_turn(board, d1)) return true;
        if (do_turn(board, d2)) return true;
    }
    return false;
}

bool runGame_fast(void) {
    Board player1Board = {6,{0,3,2,2,1,6}};
    Board player2Board = {2,{6,3,0,0,0,0}};

    for (;;) {
        if (player_turn(&player1Board)) return true;
        if (player_turn(&player2Board)) return false;
    }
}

int main(void) {
    initRandom();

    const uint64_t trials = 10000000;
    uint64_t wins = 0;

    clock_t begin = clock();
    for (uint64_t i = 0; i < trials; ++i) {
        if (runGame_fast())
            ++wins;
    }
    clock_t end = clock();

    printf("%f in %f seconds\n",
           (double)wins / (double)trials,
           (double)(end - begin) / CLOCKS_PER_SEC);
    return 0;
}
