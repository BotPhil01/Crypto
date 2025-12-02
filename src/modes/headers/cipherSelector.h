#include <stdlib.h>
#include "../../helpers/headers/types.h"
#ifndef CIPHER_H
#define CIPHER_H
extern mode strToMode(const char *pInput, const size_t sInput);
extern void selectEncCipher(cipherPtr *pCipher, mode m, bool bEnc);
extern void applyCipher(cipherSetup *cData);
#endif
