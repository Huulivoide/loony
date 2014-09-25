/*
 * util.c
 *
 * Miscellaneous functions.
 */

#include "util.h"

#include <assert.h>

int is_u8_start_byte (char c)
{
    return (c & 0xc0) != 0x80;
}

size_t u8strlen (const char *s)
{
    size_t n = 0;
    for (; *s; ++s) {
        n += (*s & 0xc0) != 0x80;
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
        *pos += (*s & 0xc0) != 0x80;
        if (*pos == n) {
            return 0;
        }
    }
    return 1;
}
