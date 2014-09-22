#pragma once

/*
 * util.h
 *
 * Miscellaneous functions.
 */

#include <stddef.h>

/* Calculates the number of codepoints in an UTF-8 encoded string. */
size_t u8strlen (const char *s);
