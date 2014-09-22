/*
 * util.c
 *
 * Miscellaneous functions.
 */

#include "util.h"

size_t u8strlen (const char *s)
{
    size_t n = 0;
    for (; *s; ++s) {
        n += (*s & 0xc0) != 0x80;
    }
    return n;
}
