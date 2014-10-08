#pragma once

/*
 * util.h
 *
 * Miscellaneous functions.
 */

#include <stddef.h>

/* Returns 1 if c is a UTF-8 start byte (including an ASCII character);
 * 0 if it isn't. */
int is_u8_start_byte (char c);

/* Returns 1 if c is a UTF-8 continuation byte; 0 if it isn't. */
int is_u8_cont_byte(char c);

/* Returns 1 if c is an ASCII character; 0 if it isn't. */
int is_u8_ascii_char(char c);

/* Returns the length of a UTF-8 character in bytes given its starting byte.
 * Returns -1 if c is not a UTF-8 start byte. */
int u8_char_length(char c);

/* Calculates the number of codepoints in an UTF-8 encoded string. */
size_t u8strlen (const char *s);

/* Finds the nth codepoint in an UTF-8 encoded string and stores its position
 * in pos. Returns 0 on success. */
int u8_find_pos(const char *s, size_t n, size_t *pos);
