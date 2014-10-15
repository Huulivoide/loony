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

#define BUFSIZE 1024

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

void display_buf(TextBuffer *buf)
{
    size_t win_h, win_w; /* window size */
    int line_digits; /* how much space required for line numbers? */
    size_t i;

    assert(buf != NULL);

    getmaxyx(stdscr, win_h, win_w);

    line_digits = buf->num_lines == 0 ? 1 : log10(buf->num_lines) + 1;

    for (i = 0; i < win_h && buf->firstrow + i < buf->num_lines; ++i) {
        size_t row = buf->firstrow + i;
        move(i, 0);
        clrtoeol();
        mvprintw(i, 0,
                "%*zu %s\n", line_digits, row+1, textbuf_get_line(buf, row));
    }
    move(textbuf_line_num(buf) - buf->firstrow,
         textbuf_col_num(buf) + line_digits + 1);
    refresh();
}

void write_new_line(TextBuffer *buf, size_t pos)
{
    int win_h, win_w;
    getmaxyx(stdscr, win_h, win_w);
    if (pos >= win_h && pos - buf->firstrow >= win_h) {
        buf->firstrow += 1;
    }

    textbuf_insert_line(buf, textline_init(""), pos);
    textbuf_move_cursor(buf, pos - textbuf_line_num(buf), INT_MIN);
    insert_at_cursor(buf);
}

void insert_at_cursor(TextBuffer *buf)
{
    int c;

    display_buf(buf);

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
        display_buf(buf);
    }
}
