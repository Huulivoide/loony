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
#include "filebuf.h"

int main (int argc, char *argv[])
{
#define BUFSIZE 1024
    FileBuffer *fbuf = filebuf_init();

    if (argc != 2) {
        printf("Loony must be launched with 'loony filename'\n");
        return 1;
    }

    if (filebuf_load_file(fbuf, argv[1])) {
        filebuf_free(fbuf);
        fbuf = filebuf_init();
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

        display_buf(fbuf);

        ch = getch();

        if (ch == 'q') {
            goto end;
        } else if (ch == 'w') {
            filebuf_save_file(fbuf, argv[1]);
        } else if (ch == 'h') {
            filebuf_move_cursor(fbuf, 0, -1);
        } else if (ch == 'l') {
            filebuf_move_cursor(fbuf, 0, 1);
        } else if (ch == 'j') {
            filebuf_move_cursor(fbuf, 1, 0);
        } else if (ch == 'k') {
            filebuf_move_cursor(fbuf, -1, 0);
        } else if (ch == 'o') {
            if (fbuf->num_lines == 0) {
                write_new_line(fbuf, fbuf->crow);
            } else {
                write_new_line(fbuf, fbuf->crow+1);
            }
            filebuf_move_cursor(fbuf, 1, 0);
        } else if (ch == 'O') {
            write_new_line(fbuf, fbuf->crow);
        } else if (ch == 'd') {
            if (fbuf->num_lines != 0) {
                filebuf_delete_line(fbuf, fbuf->crow);
            }
        } else if (ch == 'x') {
            if (fbuf->num_lines != 0) {
                filebuf_delete_char(fbuf);
            }
        }
    }

end:
    endwin();
    filebuf_free(fbuf);
    return 0;
}
