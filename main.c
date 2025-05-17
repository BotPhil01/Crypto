#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "helpers/headers/types.h"
#include "helpers/headers/blocks.h"
#include "helpers/headers/printer.h"

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
    char *cpPlaintext;
    char *cpCiphertext;
    size_t sKeySize;
    char *cpKey;
} InputData;

/*
 * @param ipDst pointer to data structure
 * @return 0 if parsing succeeded 1 otherwise
 */
int _parseInput(InputData *ipDst, const i32 iArgc, const char **cppArgv) {
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
    const char *cpPlaintext = cppArgv[1];
    FILE *fpPlaintext = fopen(cpPlaintext, "r");
    if (!fpPlaintext) { 
        return _usage(UFILENOTFOUND);
    }
    fclose(fpPlaintext);

    const char *cpCiphertext = cppArgv[2];
    FILE *fpCiphertext = fopen(cpCiphertext, "w");
    fclose(fpCiphertext);

    ipDst->cpPlaintext = cpPlaintext;
    ipDst->cpCiphertext = cpCiphertext;
    ipDst->cpKey = cpKey;
    ipDst->sKeySize = sKeySize;
    return 0;
}

size_t _fileSize(const char *cpFile) {
    struct stat s;
    stat(cpFile, &s);
    return s.st_size;
}

/*
 *
 * @return 0 if everything went ok 1 otherwise
 */
int _readData(i8 *ipDst, char *cpFileName, size_t sFileSize) {
    FILE *fpStream = fopen(cpFileName, "r");
    if (!fpStream) {
        return _usage(UFILENOTFOUND);
    }
    i32 iBytesRead = fread(ipDst, 1, sFileSize, fpStream);
    fclose(fpStream);
    if (iBytesRead != sFileSize) {
        printf("Failed to read plaintext\n");
        return 1;
    }
    return 0;
}

int _writeData(char *cpFileName, i8 *pBytes, size_t sBytesSize) {
    FILE *fd = fopen(cpFileName, "w");
    i32 iWriteCount = fwrite(pBytes, 1, sBytesSize, fd);
    return (sBytesSize == iWriteCount);
}

int main(const i32 iArgc, const char **cppArgv) {
    // take a plaintext file and write to a stdout file
    // signature:
    // TODO implement modes later
    InputData data;
    if (_parseInput(&data, iArgc, cppArgv)) {
        return 1;
    }


    // read all the data in file
    const size_t sFileSize = _fileSize(data.cpPlaintext);
    i8 iPBytes[sFileSize];
    bzero(iPBytes, sFileSize);
    if(_readData(iPBytes, data.cpPlaintext, sFileSize)) {
        return 1;
    }


    // split into blocks
    const i32 iBlockCount = blockCount(sFileSize);
    block iBlocks[iBlockCount];
    fillBlocks(iBlocks, iPBytes, sFileSize);
    // parse into algorithm
    
    const size_t sCBytes = iBlockCount * BLOCKSIZE;
    i8 iCBytes[sCBytes];
    bzero(iCBytes, sCBytes);
    blocks2Bytes(iCBytes, iBlocks, iBlockCount);


    // write data
    _writeData(data.cpCiphertext, iCBytes, sCBytes);
}
