#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static void pkv_get_checksum(const char* s, char* out) {
    char left  = 0x56;
    char right = 0xAF;

    const size_t input_len = strlen(s);
    if (input_len > 0) {
        for (int i = 0; i < input_len; i++) {
            right += s[i];
            if (right > 0xFF) {
                right -= 0xFF;
            }
            left += right;
            if (left > 0xFF) {
                left -= 0xFF;
            }
        }
    }

    const uint16_t sum = (left << 8) + right;
    snprintf(out, 5, "%04x", sum);
}

static uint8_t pkv_get_byte(const int32_t seed, uint8_t a, uint8_t b, const uint8_t c) {
    a = a % 25;
    b = b % 3;
    if (a % 2 == 0) {
        return ((seed >> a) & 0xFF) ^ ((seed >> b) | c);
    }
    return ((seed >> a) & 0xFF) ^ ((seed >> b) & c);
}

static void strsub(char* dest, const char* src, int start, int end) {
    const size_t len = strlen(src);
    if (start < 0)
        start = 0;
    if (end > len)
        end = len;

    int j = 0;
    for (int i = start; i < end && src[i] != '\0'; i++) {
        dest[j++] = src[i];
    }
    dest[j] = '\0';
}

static void strrmchr(const char* str, char* out, const char c) {
    const int len = strlen(str);
    int j         = 0;
    for (int i = 0; i < len; i++) {
        if (str[i] != c) {
            out[j] = str[i];
            j++;
        }
    }
}

static int rand_range(const int min, const int max) {
    srand(time(NULL));
    return min + rand() % (max - min + 1);
}
