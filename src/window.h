/**
 * @file window.h
 * @author dreamyeyed
 *
 * A LoonyWindow represents a visible window in the program. A window always
 * contains exactly one TextBuffer.
 */

#pragma once

#include <curses.h>

#include "textbuf.h"

/** maximum length of statusbar text */
#define STATUSBAR_LENGTH 256

typedef struct LoonyWindow
{
    /** the textbuffer that is visible in this window */
    TextBuffer *buffer;
    /** the curses window that this window should be drawn in */
    WINDOW *window;
    /** first row displayed on screen */
    int firstrow;
    /** non-zero if the screen should be completely redrawn */
    int redraw_needed;
    /** text on the statusbar */
    char statusbar_text[STATUSBAR_LENGTH];
} LoonyWindow;

/**
 * Creates a new LoonyWindow.
 *
 * @param buf the TextBuffer that is visible in this window
 * @param win the curses window that this window is drawn in
 * @return pointer to a dynamically allocated window, or NULL in case of error
 */
LoonyWindow *loonywin_init(TextBuffer *buf, WINDOW *win);

/**
 * Frees a LoonyWindow.
 *
 * @param win the window to be freed
 */
void loonywin_free(LoonyWindow *win);

/**
 * Moves the cursor in a window.
 *
 * @param win
 * @param dy Vertical movement. Negative numbers move up, positive numbers move
 * down. INT_MIN or INT_MAX move the cursor to the first or last line
 * respectively.
 * @param dx Horizontal movement. Similar to dy: negative numbers move left,
 * positive numbers move right and INT_MIN and INT_MAX move to first or last
 * column. The last column is past the last character on the line.
 */
void loonywin_move_cursor(LoonyWindow *win, int dy, int dx);

/**
 * Sets the text on the statusbar.
 *
 * @param win
 * @param text the new text
 */
void loonywin_set_statusbar(LoonyWindow *win, const char *text);

/**
 * Returns the buffer shown in the window.
 *
 * @param win
 * @return pointer to a TextBuffer or NULL in case of error
 */
TextBuffer *loonywin_get_buffer(LoonyWindow *win);
