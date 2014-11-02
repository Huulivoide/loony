/**
 * @file cursesio.h
 *
 * Input / output using the curses library.
 */

#pragma once

#include <curses.h>

#include "textbuf.h"

/**
 * Shows the contents of the given buffer on the screen.
 *
 * @param buf the buffer to be displayed
 */
void display_buf(TextBuffer *buf);

/**
 * Allows the user to write a new line of text.
 *
 * @param buf the buffer where the new text should be added
 * @param pos the line number of the new line
 */
void write_new_line(TextBuffer *buf, size_t pos);

/**
 * Inserts more text at the cursor.
 *
 * @param buf the buffer where the next text should be added
 */
void insert_at_cursor(TextBuffer *buf);
