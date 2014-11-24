/**
 * @file cursesio.h
 * @author dreamyeyed
 *
 * Input / output using the curses library.
 */

#pragma once

#include <curses.h>

#include "window.h"

/**
 * Shows the contents of the given window on the screen.
 *
 * @param win the window to be displayed
 */
void display_win(LoonyWindow *win);

/**
 * Allows the user to write a new line of text.
 *
 * @param win the window where the new text should be added
 * @param pos the line number of the new line
 */
void write_new_line(LoonyWindow *win, size_t pos);

/**
 * Inserts more text at the cursor.
 *
 * @param win the window where the next text should be added
 */
void insert_at_cursor(LoonyWindow *win);
