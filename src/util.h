/**
 * @file util.h
 * @author dreamyeyed
 *
 * Miscellaneous functions.
 */
#pragma once

#include <stddef.h>

/**
 * Checks if the given byte is a UTF-8 start byte.
 *
 * @param c the byte to check
 * @return 1 if c is a start byte, 0 otherwise
 */
int is_u8_start_byte (char c);

/**
 * Checks if the given bytes in a UTF-8 continuation byte.
 *
 * @param c the byte to check
 * @return 1 if c is a continuation byte, 0 otherwise
 */
int is_u8_cont_byte(char c);

/**
 * Checks if the given byte is an ASCII character.
 *
 * @param c the byte to check
 * @return 1 if c is an ASCII character, 0 otherwise
 */
int is_u8_ascii_char(char c);

/**
 * Returns the length of a UTF-8 codepoint in bytes.
 *
 * @param c a start byte
 * @return length of the codepoint in bytes if c is a start byte, -1 otherwise
 */
int u8_char_length(char c);

/**
 * Calculates the number of codepoints in a UTF-8 encoded string.
 *
 * @param s an UTF-8 encoded string
 * @return number of codepoints in the given string
 */
size_t u8strlen (const char *s);

/**
 * Finds the nth codepoint in a UTF-8 encoded string.
 *
 * @param s an UTF-8 encoded string
 * @param n the codepoint to find
 * @param pos an address where the correct position is stored
 * @return 0 if successful, non-zero otherwise
 */
int u8_find_pos(const char *s, size_t n, size_t *pos);
