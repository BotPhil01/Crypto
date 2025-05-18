#include <stdlib.h>
#include <assert.h>
#include "../helpers/headers/types.h"

// ALL REQUIRE A KEY
// CBC, CTR REQUIRE AN IV

mode strToMode(const char *pInput, const size_t sInput) {
    assert(sInput == 3);
    if (pInput[0] == 'E') {
        return ECB;
    }
    if (pInput[1] == 'B') {
        return CBC;
    }
    return CTR;
}

void selectEncCipher(void (*pCipher) (cipherInput), mode mode, bool bEnc) {
    if (bEnc) {
        switch (mode) {
            case ECB:
                break;
                pCipher = &
            case CBC:
                    break;
            case CTR:
                    break;
        }
}

