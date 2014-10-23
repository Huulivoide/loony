#pragma once

/*
 * cursesio.h
 *
 * This file is used for I/O using the curses library.
 */

#include <curses.h>

#include "textbuf.h"

/* Shows the contents of buf on the screen. */
void display_buf(TextBuffer *buf);

/* Allows the user to add a new line in the given position. */
void write_new_line(TextBuffer *buf, size_t pos);

/* Inserts more text at the cursor. */
void insert_at_cursor(TextBuffer *buf);
