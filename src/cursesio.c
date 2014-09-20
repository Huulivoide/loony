/*
 * cursesio.c
 *
 * This file is used for I/O using the curses library.
 */

#include "cursesio.h"

#include <assert.h>
#include <math.h>

void display_buf (const FileBuffer *buf)
{
    size_t win_h, win_w; /* window size */
    int line_digits; /* how much space required for line numbers? */
    size_t i;

    assert(buf != NULL);

    getmaxyx(stdscr, win_h, win_w);

    line_digits = buf->num_lines == 0 ? 1 : log10(buf->num_lines) + 1;

    for (i = 0; i < win_h && buf->crow + i < buf->num_lines; ++i) {
        size_t row = buf->crow + i;
        mvprintw(row, 0,
                "%*lu %s\n", line_digits, row+1, buf->lines[row]->text);
    }
    refresh();
}
