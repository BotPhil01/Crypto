#include "headers/types.h"
#include "headers/consts.h"

u8 subFwd(const u8 word) {
    return uFwdSBox[word];
}

u8 subBwd(const u8 word) {
    return uBwdSBox[word];
}
