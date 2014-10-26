/*
 * main.c
 *
 * This file contains the main() function and other stuff used at startup.
 */

#include <ctype.h>
#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <curses.h>

#include "cursesio.h"
#include "textbuf.h"

int main (int argc, char *argv[])
{
#define BUFSIZE 1024
    TextBuffer *tbuf = textbuf_init();

    if (argc != 2) {
        printf("Loony must be launched with 'loony filename'\n");
        return 1;
    }

    if (textbuf_load_file(tbuf, argv[1])) {
        textbuf_free(tbuf);
        tbuf = textbuf_init();
    }

    /* set the (hopefully) correct locale */
    setlocale(LC_ALL, "");

    /* start curses */
    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    noecho();

    for (;;) {
        int ch;

        textbuf_set_statusbar(tbuf, "Loony ALPHA");
        display_buf(tbuf);

        ch = getch();

        if (ch == 'q') {
            goto end;
        } else if (ch == 'w') {
            textbuf_save_file(tbuf, argv[1]);
        } else if (ch == 'h') {
            textbuf_move_cursor(tbuf, 0, -1);
        } else if (ch == 'l') {
            textbuf_move_cursor(tbuf, 0, 1);
        } else if (ch == 'j') {
            textbuf_move_cursor(tbuf, 1, 0);
        } else if (ch == 'k') {
            textbuf_move_cursor(tbuf, -1, 0);
        } else if (ch == 'i') {
            insert_at_cursor(tbuf);
        } else if (ch == 'o') {
            if (tbuf->num_lines == 0) {
                write_new_line(tbuf, tbuf->crow);
            } else {
                write_new_line(tbuf, tbuf->crow+1);
            }
        } else if (ch == 'O') {
            write_new_line(tbuf, tbuf->crow);
        } else if (ch == 'd') {
            if (tbuf->num_lines != 0) {
                textbuf_delete_line(tbuf, tbuf->crow);
            }
        } else if (ch == 'x') {
            if (tbuf->num_lines != 0) {
                textbuf_delete_char(tbuf);
            }
        }
    }

end:
    endwin();
    textbuf_free(tbuf);
    return 0;
}
