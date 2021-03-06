/*
 * cursesio.c
 *
 * This file is used for I/O using the curses library.
 */

#include "cursesio.h"

#include <assert.h>
#include <limits.h>
#include <math.h>

#include "util.h"

/* Reads a complete UTF-8 character to buf with getch().
 * buf must be at least 5 bytes (UTF-8 characters can be up to four bytes
 * and the character will be null terminated).
 * Returns 0 on success. */
static int read_u8_char(char *buf)
{
    int c = getch();
    int num_bytes;
    int i;

    num_bytes = u8_char_length(c);
    if (num_bytes == -1) {
        fprintf(stderr, "read a byte %d that's not a utf-8 start byte\n", c);
        return 1;
    }

    buf[0] = c;
    buf[num_bytes] = '\0';
    for (i = 1; i < num_bytes; ++i) {
        buf[i] = getch();
    }
    return 0;
}

/* Calculates the x-position of cursor on the given line. Tabs make this
 * surprisingly difficult. Wide characters are not handled yet. */
static size_t actual_column(const char *line, size_t cursor_x)
{
    size_t column = 0;

    while (cursor_x-- > 0) {
        if (*line == '\t') {
            column += TABSIZE - column % TABSIZE;
        } else {
            ++column;
        }
        do {
            ++line;
        } while (is_u8_cont_byte(*line));
    }

    return column;
}

void display_win(LoonyWindow *win)
{
    size_t win_h, win_w; /* window size */
    int line_digits; /* how much space required for line numbers? */
    size_t i;
    TextBuffer *buf;
    size_t curr_line_num;
    const char *curr_line_text;

    assert(win != NULL);

    buf = loonywin_get_buffer(win);
    curr_line_num = textbuf_line_num(buf);
    curr_line_text = textbuf_get_line(buf, curr_line_num);

    getmaxyx(win->window, win_h, win_w);

    if (win->redraw_needed) {
        erase();
        win->redraw_needed = 0;
    }

    for (i = 0; i < win_h - 1 && win->firstrow + i < buf->num_lines; ++i) {
        size_t row = win->firstrow + i;
        move(i, 0);
        clrtoeol();
        mvprintw(i, 0,
                "%*zu\t%s\n", TABSIZE-1, row+1, textbuf_get_line(buf, row));
    }

    /* draw statusbar */
    mvaddstr(win_h-1, 0, win->statusbar_text);

    move(textbuf_line_num(buf) - win->firstrow,
         TABSIZE + actual_column(curr_line_text, buf->ccol));
    refresh();
}

void write_new_line(LoonyWindow *win, size_t pos)
{
    int win_h, win_w;
    TextBuffer *buf;
    
    assert(win != NULL);

    getmaxyx(win->window, win_h, win_w);
    buf = loonywin_get_buffer(win);

    if (pos >= win_h && pos - win->firstrow >= win_h) {
        win->firstrow += 1;
    }

    textbuf_insert_line(buf, textline_init(""), pos);
    textbuf_move_cursor(buf, pos - textbuf_line_num(buf), INT_MIN);
    insert_at_cursor(win);
}

void insert_at_cursor(LoonyWindow *win)
{
    int c;
    TextBuffer *buf;

    assert(win != NULL);

    buf = loonywin_get_buffer(win);
    display_win(win);

    while ((c = getch()) != 27) { /* 27 = escape */
        char tmp[5];
        int line = textbuf_line_num(buf);
        int col = textbuf_col_num(buf);
        if (c == KEY_BACKSPACE) {
            if (col > 0) {
                textbuf_move_cursor(buf, 0, -1);
                textbuf_delete_char(buf);
            } else if (line > 0) {
                textbuf_join_with_next_line(buf, line - 1);
            }
        } else if (c == '\n') {
            textbuf_split_line(buf, line, col);
            textbuf_move_cursor(buf, 1, INT_MIN);
        } else {
            /* not a special character */
            ungetch(c);
            if (read_u8_char(tmp) != 0) {
                /* error */
                return;
            }
            textbuf_insert_at_cursor(buf, tmp);
        }
        display_win(win);
    }
}
