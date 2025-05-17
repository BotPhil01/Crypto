#include "headers/types.h"
#include <stdlib.h>
#include <stdio.h>

void printArr(i8 *iArr, const size_t sArr) {
    u32 counter = 0;
    while (counter != sArr) {
        printf("%i, ", iArr[counter++]);
    }
    printf("\n");
}
