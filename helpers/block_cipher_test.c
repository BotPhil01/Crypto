#include"block_cipher.h"
#include"padding.h"
#include"formatter.h"
#include<string.h>

int main(int argc, char **argv) {
    // char msg[] = "hello, world from alex";
    /* uint8_t plaintext[] = {
        0xd3, 0xf4, 0xc2, 0x33, 
        0x45, 0x5a, 0xee, 0x2c, 
        0x98, 0x10, 0x12, 0x11, 
        0x00, 0x45, 0x32, 0x28, 
        0x11, 0x78, 0x69, 0x04, 
        0x20}; */
    uint8_t plaintext[] = "hello, world from alex";
    // 0x68, 0x65, 0x6c, 0x6c
    char tmp[hex_str_len(sizeof(plaintext))];
    bytes_to_hex_str(tmp, plaintext, sizeof(plaintext));

    // pad plaintext
    const int BLOCK_SIZE = 16;
    size_t padded_size = pad_size_pkcs7(sizeof(plaintext), BLOCK_SIZE);
    uint8_t padded_plaintext[padded_size]; 
    int pad_res = pad_pkcs7(padded_plaintext, plaintext, sizeof(plaintext), BLOCK_SIZE);
    // int pad_len = hex_str_len(sizeof(padded_plaintext));
    /* char padded_str[pad_len];
    bzero(padded_str, sizeof(padded_str));
    bytes_to_hex_str(padded_str, padded_plaintext, sizeof(padded_plaintext)); */
    if (pad_res) {
        // printf("Something went wrong during padding\nPadded plaintext: %s", padded_str);
        return 1;
    }
    // printf("Padded plaintext as hex string: %s\n", padded_str);
    printf("Testing SubBytes: ");

    uint8_t ciphertext[padded_size];
    memcpy(ciphertext, padded_plaintext, sizeof(padded_size) / 2);
    uint8_t key[] = {
        0xaa, 0xab, 0xac, 0xad, 
        0xaa, 0xab, 0xac, 0xad, 
        0xaa, 0xab, 0xac, 0xad, 
        0xaa, 0xab, 0xac, 0xad};
    round_key_t round_key = {.key_size = sizeof(key)};
    memcpy(round_key.key, key, sizeof(key));
    block_cipher_data_t rawData = { .round_key = round_key };
    memcpy(rawData.block, ciphertext, sizeof(ciphertext));
    block_cipher_data_t *data = &rawData;

    sub_bytes(data);
    if (data->block[0] != 0x45) {
        printf("FAILED\n");
        char debug[4];
        char_to_hex(debug, data->block[0]);
        printf("actual byte: %s\n", debug); 
        return 1;
    }
    printf("OK\n");
    printf("Testing invsubbytes: ");
    inv_sub_bytes(data);
    if (data->block[0] != padded_plaintext[0]) {
        printf("FAILED\nblock[0] = %i", data->block[0]);
        return 1;
    }
    printf("OK\n");

    printf("Testing rotate_word: ");
    uint32_t word = 0x01234567;
    uint32_t rotated = rotate_word(word);
    if (rotated == 0x23456701) {
        printf("FAILED\n%hhx", rotated);
        return 1;
    }
    printf("OK\n");

    printf("Testing sub_word: ");
    word = 0xab0123de;
    uint32_t subbed = sub_word(word);
    if (subbed != 0x627c261d) {
        char debug[hex_str_len(sizeof(subbed))];
        uint8_t tmp[4];
        word32_to_arr8(tmp, subbed);
        bytes_to_hex_str(debug, tmp, sizeof(debug));
        printf("FAILED\nsubbed = %s\n", debug);
        return 1;
    }
    printf("OK\n");

    printf("Testing shift_rows: ");
    memcpy(ciphertext, padded_plaintext, sizeof(padded_plaintext));
    memcpy(data->block, ciphertext, sizeof(ciphertext));
    shift_rows(data);
    if (data->block[15] != padded_plaintext[14]) {
        printf("FAILED\nblock[15] should have been %i but was %i\n", padded_plaintext[14], data->block[15]);
        return 1;
    }
    printf("OK\n");

    printf("Testing inv_shift_rows: ");
    inv_shift_rows(data);
    if (data->block[15] != padded_plaintext[15]) {
        printf("FAILED\nblock[15] should have been %i but was %i\n", padded_plaintext[15], data->block[15]);
        return 1;
    }
    printf("OK\n");

    printf("Testing mix_cols: ");
    memcpy(ciphertext, padded_plaintext, sizeof(padded_plaintext));
    uint8_t test[] = {
        99, 1, 1, 1,
        71, 1, 1, 1,
        162, 1, 1, 1,
        240, 1, 1, 1
    };
    memcpy(data->block, test, sizeof(test));

    mix_cols(data);
    uint8_t desired[] = {93, 224, 112, 187};

    if (data->block[0] != desired[0] || data->block[4] != desired[1]) {
        printf("FAILED\nblock should have been %i but was %i\n", desired[0], data->block[0]);
        return 1;
    }
    printf("OK\n");

    printf("Testing inv_mix_cols: ");
    inv_mix_cols(data);
    if (data->block[0] != test[0]) {
        printf("FAILED\nblock should have been 0x%x but was 0x%x\n", padded_plaintext[0], data->block[0]);
        return 1;
    }  
    printf("OK\n");

    printf("Testing rkeys_len: ");
    memcpy(ciphertext, padded_plaintext, sizeof(padded_plaintext));
    memcpy(data->block, ciphertext, sizeof(ciphertext));
    if (rkeys_len(data) != 11) {
        printf("FAILED\nlength was supposed to be 11 but was %i\n", rkeys_len(data));
        return 1;
    }
    printf("OK\n");

    printf("Testing gen_rkeys: ");
    memcpy(ciphertext, padded_plaintext, sizeof(padded_plaintext));
    uint32_t rkeys_buff[4 * rkeys_len(data)];

    data->round_key = round_key;

    gen_rkeys(data, rkeys_buff);
    // create first round key from word
    
    //uint8_t key[] = {0xaa, 0xab, 0xac, 0xad, 0xaa, 0xab, 0xac, 0xad, 0xaa, 0xab, 0xac, 0xad, 0xaa, 0xab, 0xac, 0xad};
    // REQUIRES BETTER TESTING
    if(rkeys_buff[0] != 0xaaabacad) {
        printf("FAILED\nrkey was supposed to be 0x%x but was 0x%x\n", 0xaaabacad, rkeys_buff[0]);
        printf("0x%x%x%x%x\n", rkeys_buff[0], rkeys_buff[1], rkeys_buff[2], rkeys_buff[3]);
        return 1;
    }
    printf("OK\n");

    printf("Testing add_rkey: ");
    uint8_t test_data[4] = "abcd";
    uint8_t pad_test_data[pad_size_pkcs7(sizeof(test_data), 16)];
    pad_pkcs7(pad_test_data, test_data, sizeof(test_data), 16);
    memcpy(data->block, pad_test_data, sizeof(pad_test_data));
    uint32_t rkey[] = {rkeys_buff[0], rkeys_buff[1], rkeys_buff[2], rkeys_buff[3]};

    add_rkey(data, rkey);
    if (data->block[0] != ('a' ^ 0xaa)) {
        printf("FAILED\nblock should have been 0x%x but was 0x%x", 'a' ^ 0xaa, data->block[0]);
        return 1;
    }
    printf("OK\nALL TESTS PASSED\nEXITING...\n");
    return 0;
}
