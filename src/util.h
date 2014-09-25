#pragma once

/*
 * util.h
 *
 * Miscellaneous functions.
 */

#include <stddef.h>

/* Returns 1 if c is a UTF-8 start byte; 0 if it isn't. */
int is_u8_start_byte (char c);

/* Calculates the number of codepoints in an UTF-8 encoded string. */
size_t u8strlen (const char *s);

/* Finds the nth codepoint in an UTF-8 encoded string and stores its position
 * in pos. Returns 0 on success. */
int u8_find_pos(const char *s, size_t n, size_t *pos);
