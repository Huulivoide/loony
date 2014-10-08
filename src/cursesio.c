/*
 * cursesio.c
 *
 * This file is used for I/O using the curses library.
 */

#include "cursesio.h"

#include <assert.h>
#include <math.h>

#include "util.h"

#define BUFSIZE 1024

/* Reads a complete UTF-8 character to buf with getch().
 * buf must be at least 5 bytes (UTF-8 characters can be up to four bytes
 * and the character will be null terminated).
 * If the character is a backspace or some other special character, that
 * character is returned.
 * Returns -1 on error. */
static int read_u8_char(char *buf)
{
    int c = getch();
    int num_bytes;
    int i;

    if (c == KEY_BACKSPACE) {
        return c;
    }

    num_bytes = u8_char_length(c);
    if (num_bytes == -1) {
        fprintf(stderr, "read a byte %d that's not a utf-8 start byte\n", c);
        return -1;
    }

    buf[0] = c;
    buf[num_bytes] = '\0';
    for (i = 1; i < num_bytes; ++i) {
        buf[i] = getch();
    }
    return 0;
}

void display_buf (FileBuffer *buf)
{
    size_t win_h, win_w; /* window size */
    int line_digits; /* how much space required for line numbers? */
    size_t i;

    assert(buf != NULL);

    if (buf->redraw_needed) {
        clear();
        buf->redraw_needed = 0;
    }

    getmaxyx(stdscr, win_h, win_w);

    line_digits = buf->num_lines == 0 ? 1 : log10(buf->num_lines) + 1;

    for (i = 0; i < win_h && buf->firstrow + i < buf->num_lines; ++i) {
        size_t row = buf->firstrow + i;
        move(i, 0);
        clrtoeol();
        mvprintw(i, 0,
                "%*zu %s\n", line_digits, row+1, buf->lines[row]->text);
    }
    move(buf->crow - buf->firstrow, buf->ccol + line_digits + 1);
    refresh();
}

void write_new_line (FileBuffer *buf, size_t pos)
{
    int win_h, win_w;
    getmaxyx(stdscr, win_h, win_w);
    if (pos >= win_h && pos - buf->firstrow >= win_h) {
        buf->firstrow += 1;
    }

    filebuf_insert_line(buf, fileline_init(""), pos);
    buf->crow = pos;
    buf->ccol = 0;
    insert_at_cursor(buf);
}

void insert_at_cursor(FileBuffer *buf)
{
    char tmp[5];
    int c;

    display_buf(buf);

    while ((c = read_u8_char(tmp)) != -1) {
        if (tmp[0] == 27) { /* 27 = escape */
            return;
        } else if (c == KEY_BACKSPACE) {
            if (buf->ccol > 0) {
                (buf->ccol)--;
                filebuf_delete_char(buf);
            } else if (buf->crow > 0) {
                filebuf_join_with_next_line(buf, buf->crow-1);
                goto end_loop;
            }
            goto end_loop;
        }

        fileline_insert(buf->lines[buf->crow], tmp, buf->ccol);
        (buf->ccol)++;
end_loop:
        display_buf(buf);
    }
}
