#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "pkv.h"

#define BUFFER_SIZE 64
#define CHECK_BIT(x, n) ((x) & (1 << (n)))

static const uint8_t key_check_pairs[6] = {
  0b0011,  // 3
  0b0101,  // 5
  0b1001,  // 9
  0b0110,  // 6
  0b1010,  // 10
  0b1100,  // 12
};

bool pkv_check_checksum(const char* key, char* fmt_key_out) {
    if (strlen(key) != 24) {
        return false;
    }

    char key_str[21] = {'\0'};
    strrmchr(key, key_str, '-');
    strcpy(fmt_key_out, key_str);

    char checksum[5] = {'\0'};
    strsub(checksum, key_str, 16, 20);

    char pre_checksum[17] = {'\0'};
    strsub(pre_checksum, key_str, 0, 16);

    char out_checksum[5] = {'\0'};
    pkv_get_checksum(pre_checksum, out_checksum);

    return strcmp(checksum, out_checksum) == 0;
}

bool pkv_check_key(const char* key) {
    char key_str[21];
    if (!pkv_check_checksum(key, key_str)) {
        fprintf(stderr, "Checksum invalid\n");
        return false;
    }

    char seed[9] = {'\0'};
    strsub(seed, key_str, 0, 8);

    const int32_t seed_i64         = strtoll(seed, NULL, 16);
    const uint8_t check_bits       = key_check_pairs[rand_range(0, 5)];
    const int checkbit_values_a[4] = {24, 10, 1, 7};
    const int checkbit_values_b[4] = {3, 0, 2, 1};
    const int checkbit_values_c[4] = {200, 56, 91, 100};
    const int start_indices[4]     = {8, 10, 12, 14};
    const int end_incides[4]       = {10, 12, 14, 16};

    for (int i = 0; i < 4; i++) {
        if (CHECK_BIT(check_bits, i)) {
            char checkbit[3] = {'\0'};
            strsub(checkbit, key_str, start_indices[i], end_incides[i]);
            const uint8_t bit_u8 = pkv_get_byte(seed_i64,
                                                checkbit_values_a[i],
                                                checkbit_values_b[i],
                                                checkbit_values_c[i]);
            char bit[3]          = {'\0'};
            snprintf(bit, 3, "%x", bit_u8);
            if (strcmp(bit, checkbit) != 0) {
                fprintf(stderr, "Check bit [%d] invalid\n", i + 1);
                return false;
            }
        }
    }

    return true;
}

int main(int argc, char* argv[]) {
    bool result = false;

    if (argc == 2) {
        result = pkv_check_key(argv[1]);
    } else {
        puts("Enter your serial number: ");
        char buffer[BUFFER_SIZE] = {'\0'};
        fgets(buffer, BUFFER_SIZE, stdin);

        const int len   = strlen(buffer);
        buffer[len - 1] = '\0';
        result          = pkv_check_key(buffer);
    }

    if (result) {
        puts("Serial number is valid!");
    } else {
        puts("Serial number is invalid. Please try again.");
    }

    return 0;
}
