#include<stdint.h>
#include<stdio.h>
//#include"padding.h"

int foo(uint8_t *arr, int n) {
    for (int i = 0; i < n; i++) {
        printf("%i", arr[i]);
    }
    return 0;
}

//int pad_bytes(uint8_t *bytes[], size_t length, int mode);
int main(int argc, char **argv) {
    uint8_t arr[] = {1, 2, 3, 4, 5};
    return foo(arr, 5);
}
