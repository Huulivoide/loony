#pragma once

/*
 * cursesio.h
 *
 * This file is used for I/O using the curses library.
 */

#include <curses.h>

#include "filebuf.h"

/* Shows the contents of buf on the screen. */
void display_buf (const FileBuffer *buf);
