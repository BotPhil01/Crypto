#include "../helpers/headers/types.h"
#include "../helpers/headers/cipher.h"
#include "../helpers/headers/printer.h"
#include <stdio.h>


void ecbEncrypt(cipherInput *pInput) {
    for (u32 i = 0; i < pInput->sBlocks; i++) {
        aesEncrypt(&pInput->blocks[i], pInput->keySchedule);
    }
}

void ecbDecrypt(cipherInput *pInput) {
}
