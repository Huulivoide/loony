#pragma once

/*
 * cursesio.h
 *
 * This file is used for I/O using the curses library.
 */

#include <curses.h>

#include "filebuf.h"

/* Shows the contents of buf on the screen. */
void display_buf (FileBuffer *buf);

/* Allows the user to add a new line in the given position. */
void write_new_line (FileBuffer *buf, size_t pos);
