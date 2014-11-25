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
    TextBuffer *tbuf = textbuf_init();
    LoonyWindow *win;

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

    win = loonywin_init(tbuf, stdscr);

    for (;;) {
        int ch;

        loonywin_set_statusbar(win, "Loony ALPHA");
        display_win(win);

        ch = getch();

        if (ch == 'q') {
            goto end;
        } else if (ch == 'w') {
            textbuf_save_file(tbuf, argv[1]);
        } else if (ch == 'h') {
            loonywin_move_cursor(win, 0, -1);
        } else if (ch == 'l') {
            loonywin_move_cursor(win, 0, 1);
        } else if (ch == 'j') {
            loonywin_move_cursor(win, 1, 0);
        } else if (ch == 'k') {
            loonywin_move_cursor(win, -1, 0);
        } else if (ch == 'i') {
            insert_at_cursor(win);
        } else if (ch == 'o') {
            write_new_line(win, tbuf->crow+1);
        } else if (ch == 'O') {
            write_new_line(win, tbuf->crow);
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
    loonywin_free(win);
    textbuf_free(tbuf);
    return 0;
}
