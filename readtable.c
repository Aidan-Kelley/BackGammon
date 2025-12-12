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

float readAverageTurnsTable(const char* src) {
    FILE *f = fopen(src, "rb");
    char state[6] = {1,1,1,1,1,1};
    uint32_t idx = compressBoard(state);
    fseek(f, (idx) * sizeof(float), SEEK_SET);
    float averageTurns;
    fread(&averageTurns,sizeof(float), 1, f);
    return averageTurns;
}

int main() {
    
    FILE *f = fopen("winspercents.bin","rb");
    char state[6] = {0,3,1,0,0,0};
    uint32_t targetId = compressBoard(state);
    printf("%d\n",targetId);
    uint32_t id;
    for (uint32_t i = 0; 1; i++) {
        if (fseek(f,i*92,SEEK_SET) != 0) {
            printf("Not found %d",i);
            return 1;
        }
        fread(&id,sizeof(uint32_t), 1, f);
        if(id == targetId)
            break;
    }
    for (int i = 0; i < 22; i++) {
        float out;
        fread(&out,sizeof(float), 1, f);
        printf("%f\n",out);
    }
    return 0;
}