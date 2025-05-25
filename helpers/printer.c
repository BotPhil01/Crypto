#include "headers/types.h"
#include <stdlib.h>
#include <stdio.h>

void printArr(u8 *iArr, const size_t sArr) {
    u32 counter = 0;
    while (counter != sArr) {
        printf("%x, ", iArr[counter++]);
    }
    printf("\n");
}

void printKeySchedule(keySchedule *ks) {
    printf("Printing key schedule\n");
    printf("Printing initial key\n");
    printArr(ks->kInit.uKey, 32);
    for (int i = 0; i < ks->sRKeys; i++) {
        printf("Printing key: %i\n", i);
        printArr(ks->rpRKeys[i].bytes, 16);
    }
    printf("\n");
}
