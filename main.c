#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef int bool;
#define true = 1;
#define false = 0;

typedef enum eUsage {
    UGENERAL,
    UKEYSIZE,
    UFILENOTFOUND,
    UKEY,
} eUsage;

int _usage(const eUsage e) {
    switch (e) {
        case UFILENOTFOUND:
            printf("Usage: plaintext file not found\n");
            break;
        case UGENERAL:
            printf("Usage: ./aes plaintext ciphertext keysize key [mode]\n");
            break;
        case UKEYSIZE:
            printf("Usage: invalid keysize value (bytes) {16, 24, 32}\n");
            break;
        case UKEY:
            printf("Usage: bad key for chosen keysize\n");
            break;
    }
    return 1;
}

bool _keySizeValid(const size_t sKeySize) {
    return sKeySize == 16 || sKeySize == 24 || sKeySize == 32;
}

bool _keyValid(const char *key, const size_t sKeySizeOpt) {
    const size_t sKeyLength = strlen(key);
    return sKeyLength == sKeySizeOpt;
}

typedef struct InputData {
    FILE *fpPlaintext;
    FILE *fpCiphertext;
    size_t sKeySize;
    char *cpKey;
} InputData;

/*
 * @param ipDst pointer to
 *
 */
int _parseInput(InputData *ipDst, const int iArgc, const char **cppArgv) {
    if (iArgc < 5) {
        return _usage(UGENERAL);
    }

    const size_t sKeySize = atoi(cppArgv[3]);
    if (!_keySizeValid(sKeySize)) {
        return _usage(UKEYSIZE);
    }
    const char *cpKey = cppArgv[4];
    if (!_keyValid(cpKey, sKeySize)) {
        return _usage(UKEY);
    }


    // check if plaintext exists
    FILE *fpPlaintext = fopen(cppArgv[1], "r");
    if (!fpPlaintext) { 
        return _usage(UFILENOTFOUND);
    }

    FILE *fpCiphertext = fopen(cppArgv[2], "w");

    ipDst->fpPlaintext = fpPlaintext;
    ipDst->fpCiphertext = fpCiphertext;
    ipDst->cpKey = cpKey;
    ipDst->sKeySize = sKeySize;
    return 0;
}

void cleanup(InputData *ipData) {
    if (ipData->fpCiphertext) {
        fclose(ipData->fpCiphertext);
    }
    if (ipData->fpPlaintext) {
        fclose(ipData->fpPlaintext);
    }
}
int main(const int iArgc, const char **cppArgv) {
    // take a plaintext file and write to a stdout file
    // signature:
    // TODO implement modes later
    InputData data;
    if (_parseInput(&data, iArgc, cppArgv)) {
        return 1;
    }
    return 0;
}
