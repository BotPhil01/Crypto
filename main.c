#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <assert.h>
#include "helpers/headers/types.h"
#include "helpers/headers/blocks.h"
#include "helpers/headers/printer.h"
#include "helpers/headers/keyscheduler.h"
#include "helpers/headers/hex.h"
#include "modes/headers/cipherSelector.h"

int _usage(const eUsage e) {
    switch (e) {
        case UFILENOTFOUND:
            printf("Usage: plaintext file not found\n");
            break;
        case UGENERAL:
            printf("Usage: ./aes plaintext ciphertext keysize key mode encrypt/decrypt [hexmode]\n");
            break;
        case UKEYSIZE:
            printf("Usage: invalid keysize value (bytes) {16, 24, 32}\n");
            break;
        case UKEY:
            printf("Usage: bad key for chosen keysize\n");
            break;
        case UINPUTSIZE:
            printf("Usage: Input limit reached stop trying to overflow my program\n");
            break;
    }
    return 1;
}

bool _keySizeValid(const size_t sKeySize) {
    return sKeySize == 16 || sKeySize == 24 || sKeySize == 32;
}

bool _keyLenValid(const char *key, const size_t sKeySizeOpt, const bool bHex) {
    const size_t sKeyLength = strlen(key);
    size_t sMultiplier = 1;
    if (bHex == true) {
        sMultiplier = 2;
    }
    return sKeyLength == sKeySizeOpt * sMultiplier;
}

/*
 * @param ipDst pointer to data structure
 * @return 0 if parsing succeeded 1 otherwise
 */
int _parseInput(InputData *ipDst, const i32 iArgc, const char **cppArgv) {
    // current signature
    // ./debug src dst keysize key mode encrypt/decrypt hexmode 
    // ./debug src dst key mode [options]
    // src = src file
    // dst = dst file
    // key = key to use
    // mode = cipher mode EG: CBC-DECRYPT
    // options:
    // -hk = hex key (treat the key as a hex value not an integer)
    // -ht = hex text (treat the text as a hex value not as a byte)
    // input size check
    if (iArgc < 7) {
        return _usage(UGENERAL);
    }

    // input length check
    for (u32 i = 0; i < iArgc; i++) {
        if(strnlen(cppArgv[i], SMAXSTRINPUT) == SMAXSTRINPUT) {
            return _usage(UINPUTSIZE);
        }
    }

    strInput inPlaintext;
    strInput inCiphertext;
    strInput inKeySize;
    strInput inKey;
    strInput inMode;
    strInput inEnc;
    strInput inHex;

    bzero(inPlaintext.cVal, SMAXSTRINPUT);
    bzero(inCiphertext.cVal, SMAXSTRINPUT);
    bzero(inKeySize.cVal, SMAXSTRINPUT);
    bzero(inKey.cVal, SMAXSTRINPUT);
    bzero(inMode.cVal, SMAXSTRINPUT);
    bzero(inEnc.cVal, SMAXSTRINPUT);
    bzero(inHex.cVal, SMAXSTRINPUT);

    memcpy(inPlaintext.cVal, cppArgv[1], strlen(cppArgv[1]));
    memcpy(inCiphertext.cVal, cppArgv[2], strlen(cppArgv[2]));
    memcpy(inKeySize.cVal, cppArgv[3], strlen(cppArgv[3]));
    memcpy(inKey.cVal, cppArgv[4], strlen(cppArgv[4]));
    memcpy(inMode.cVal, cppArgv[5], strlen(cppArgv[5]));
    memcpy(inEnc.cVal, cppArgv[6], strlen(cppArgv[6]));
    if (inEnc.cVal[0] == 'e') {
        ipDst->bEnc = true;
    } else {
        ipDst->bEnc = false;
    }
    if (iArgc == 8) {
        memcpy(inHex.cVal, cppArgv[7], strlen(cppArgv[7]));
        ipDst->bHex = false;
        if (inHex.cVal[0] == 'y') {
            ipDst->bHex = true;
        }
    }

    // check key size
    const size_t sKeySize = atoi(inKeySize.cVal);
    if (!_keySizeValid(sKeySize)) {
        return _usage(UKEYSIZE);
    }

    // check if key lengt his valid
    if (!_keyLenValid(inKey.cVal, sKeySize, ipDst->bHex)) {
        return _usage(UKEY);
    }

    // check if plaintext exists
    FILE *fpPlaintext = fopen(inPlaintext.cVal, "r");
    if (!fpPlaintext) { 
        return _usage(UFILENOTFOUND);
    }
    fclose(fpPlaintext);

    // copy into ds and return
    strcpy(ipDst->inPlaintext.cVal, inPlaintext.cVal);
    strcpy(ipDst->inCiphertext.cVal, inCiphertext.cVal);
    strcpy(ipDst->inKey.cVal, inKey.cVal);
    ipDst->sKeySize = sKeySize;
    strcpy(ipDst->inMode.cVal, inMode.cVal);

    return 0;
}

size_t _fileSize(const char *cpFile) {
    struct stat s;
    stat(cpFile, &s);
    return s.st_size - 1;
}

/*
 *
 * @return 0 if everything went ok 1 otherwise
 */
int _readData(u8 *ipDst, char *cpFileName, size_t sFileSize) {
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

int _writeData(char *cpFileName, u8 *pBytes, size_t sBytesSize) {
    FILE *fd = fopen(cpFileName, "w");
    i32 iWriteCount = fwrite(pBytes, 1, sBytesSize, fd);
    fclose(fd);
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

    cipherSetup cData;

    // read all the data in file
    size_t sFileSize = _fileSize(data.inPlaintext.cVal);
    u8 iPBytes[sFileSize];
    bzero(iPBytes, sFileSize);
    if(_readData(iPBytes, data.inPlaintext.cVal, sFileSize)) {
        return 1;
    }

    if (data.bHex == true) {
        ascToHex(iPBytes, sFileSize);
        sFileSize = sFileSize / 2;
    }

    // split into blocks
    u32 uBlockCount = blockCount(sFileSize);
    if (data.bHex == true) {
        uBlockCount = uBlockCount / 2;
    }
    cData.sBlocks = uBlockCount;

    block uBlocks[uBlockCount];
    cData.blocks = uBlocks;
    strToBlocks(cData.blocks, iPBytes, sFileSize);

    // create keys

    katob(&cData.keySchedule.kInit, &data);

    createRKeys(&cData.keySchedule);

    // parse into algorithm

    mode mode = strToMode(data.inMode.cVal, strlen(data.inMode.cVal));
    selectEncCipher(&cData.cipher, mode, data.bEnc);

    applyCipher(&cData);

    const size_t sCBytes = uBlockCount * BLOCKSIZE;
    u8 iCBytes[sCBytes];
    bzero(iCBytes, sCBytes);
    blocks2Bytes(iCBytes, cData.blocks, cData.sBlocks);

    printf("hex output\n");
    printArr(iCBytes, sCBytes);

    // write data
    _writeData(data.inCiphertext.cVal, iCBytes, sCBytes);
}
