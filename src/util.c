/*
 * util.c
 *
 * Miscellaneous functions.
 */

#include "util.h"

#include <assert.h>

int is_u8_start_byte (char c)
{
    /* c is a start byte iff the first two bytes are *not* 10.
     * iff == if and only if. That's not a typo. */
    return (c & 0xc0) != 0x80;
}

int is_u8_cont_byte(char c)
{
    /* c is a continuation byte iff the first two bits are 10. */
    return (c & 0xc0) == 0x80;
}

int is_u8_ascii_char(char c)
{
    /* c is an ASCII character iff the first bit is zero. */
    return !(c >> 7);
}

int u8_char_length(char c)
{
    if (!is_u8_start_byte(c)) {
        return -1;
    } else if (is_u8_ascii_char(c)) {
        return 1;
    } else {
        /* Length in bytes == number of 1 bits before the first 0 bit starting
         * from the left. */
        int num_bytes = 1;
        int mask;
        for (mask = (1 << 6); c & mask; mask >>= 1) {
            num_bytes++;
        }
        return num_bytes;
    }
}

size_t u8strlen (const char *s)
{
    size_t n = 0;
    for (; *s; ++s) {
        n += is_u8_start_byte(*s);
    }
    return n;
}

int u8_find_pos(const char *s, size_t n, size_t *pos)
{
    assert(s != NULL);
    assert(pos != NULL);

    *pos = 0;
    if (n == 0) {
        return 0;
    }
    for (; *s; ++s) {
        *pos += is_u8_start_byte(*s);
        if (*pos == n) {
            return 0;
        }
    }
    return 1;
}
