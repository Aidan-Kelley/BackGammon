#include <stdlib.h>
#include <stdint.h>
#include <time.h>

static uint32_t rng_state; // seed however you like
void initRandom() {
    srand(time(NULL));
    rng_state = time(NULL);
}

int randInt(int end) {
    return rand() % end;
}

static inline uint32_t fast_rand_u32(void) {
    uint32_t x = rng_state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    rng_state = x;
    return x;
}

static inline int randInt6(void) { 
    return fast_rand_u32() % 6;
}