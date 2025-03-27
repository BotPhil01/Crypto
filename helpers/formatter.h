#include <stddef.h>
#include <stdint.h>

#define length(arr, size) (size / arr[0])

/*
* Converts a char to a non null terminated string representing the hex value of a byte
* Out should have length >= 4
* @params out buffer to read to 
* @params byte the byte to convert
* @return void
*/
void char_to_hex(char *out, const uint8_t byte);

/*
* Converts an array of bytes to a hex array (mainly for debugging)
* @params out the buffer to write to
* @params bytes the bytes to convert
* @params size size of the bytes
* @returns void
*/
void bytes_to_hex_str(char *out, const uint8_t *bytes, size_t size);

/*
* Calculates the length a hex string should take including terminating character
* @params bytes_size size of the bytes array
* @returns length of string
*/
int hex_str_len(const size_t bytes_size);

/*
* Converts a 32 bit word to a 8bit array
* @params dest buffer to write to
* @params word to break
* @returns void
*/
void word32_to_arr8(uint8_t *dest, const uint32_t word);

/*
* Converts a 32 bit word to a 8bit array
* @params src to combine
* @returns combined word
*/
uint32_t arr8_to_word32(const uint8_t * const src);
