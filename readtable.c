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
    char board[6] = {0,2,3,1,3,1};
    uint32_t idx = compressBoard(board);
    printf("%d\n",idx);
    FILE *f = fopen("table.bin", "rb");

    fseek(f, (idx) * sizeof(float), SEEK_SET);
    float averageTurns;
    fread(&averageTurns,sizeof(float), 1, f);
    printf("%f",averageTurns);
    fclose(f);
    return 0;
}