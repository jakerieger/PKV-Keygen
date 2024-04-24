// Author: Jake Rieger
// Created: 4/23/2024.
//

// ************************************************************************************** //
// *      THIS FILE IS USED TO GENERATE KEYS FOR TESTING, THE CRACKME IS ONLY ONE.C     * //
// ************************************************************************************** //

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "pkv.h"

int32_t djb2_hash(const int32_t* data, const size_t len) {
    int32_t hash             = 5381;
    const uint8_t* byte_data = (const uint8_t*)data;

    for (size_t i = 0; i < len * sizeof(int32_t); i++) {
        hash = ((hash << 5) + hash) + byte_data[i];
    }

    return hash;
}

void pkv_make_key(const int32_t seed, char* key_out) {
    uint8_t key_bytes[4] = {0};
    key_bytes[0]         = pkv_get_byte(seed, 24, 3, 200);
    key_bytes[1]         = pkv_get_byte(seed, 10, 0, 56);
    key_bytes[2]         = pkv_get_byte(seed, 1, 2, 91);
    key_bytes[3]         = pkv_get_byte(seed, 7, 1, 100);

    char key_seed[8 + 1]     = {'\0'};
    char key_a[2 + 1]        = {'\0'};
    char key_b[2 + 1]        = {'\0'};
    char key_c[2 + 1]        = {'\0'};
    char key_d[2 + 1]        = {'\0'};
    char* key_parts[4]       = {key_a, key_b, key_c, key_d};
    char key_checksum[4 + 1] = {'\0'};

    snprintf(key_seed, 9, "%08x", seed);

    for (int i = 0; i < 4; ++i) {
        snprintf(key_parts[i], 3, "%02x", key_bytes[i]);
    }

    char pre_checksum[17];
    snprintf(pre_checksum, 17, "%s%s%s%s%s", key_seed, key_a, key_b, key_c, key_d);
    pkv_get_checksum(pre_checksum, key_checksum);

    char final_key[21] = {'\0'};
    snprintf(final_key, 21, "%s%s%s%s%s%s", key_seed, key_a, key_b, key_c, key_d, key_checksum);

    char key_part_A[5] = {'\0'};
    char key_part_B[5] = {'\0'};
    char key_part_C[5] = {'\0'};
    char key_part_D[5] = {'\0'};
    char key_part_E[5] = {'\0'};

    strsub(key_part_A, final_key, 0, 4);
    strsub(key_part_B, final_key, 4, 8);
    strsub(key_part_C, final_key, 8, 12);
    strsub(key_part_D, final_key, 12, 16);
    strsub(key_part_E, final_key, 16, 20);

    snprintf(key_out,
             25,
             "%s-%s-%s-%s-%s",
             key_part_A,
             key_part_B,
             key_part_C,
             key_part_D,
             key_part_E);
}

int main(int argc, char* argv[]) {
    uint32_t iterations = 1;
    if (argc == 2) {
        iterations = atoi(argv[1]);
    }

    for (int i = 0; i < iterations; i++) {
        const int32_t t    = time(NULL);
        const int32_t seed = djb2_hash(&t, sizeof(int32_t));
        char key[25]       = {'\0'};
        pkv_make_key(seed, key);

        printf("%s\n", key);
    }
    return 0;
}
