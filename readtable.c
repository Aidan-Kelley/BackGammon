#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

uint32_t compressBoard(const char* board) {
    uint32_t result = 0;
    for (int i = 0; i < 6; i++) {
        result |= board[i] << (i << 2);
    }
    return result;
}

int main() {
    
    FILE *f = fopen("table.bin", "rb");

    char state[6] = {1,1,1,1,1,1};
    uint32_t idx = compressBoard(state);
    fseek(f, (idx) * sizeof(float), SEEK_SET);
    float averageTurns;
    fread(&averageTurns,sizeof(float), 1, f);
    fclose(f);
    printf("Average delta: %f", averageTurns);
    return 0;
}