/*
 * main.c
 *
 * This file contains the main() function and other stuff used at startup.
 */

#include <ctype.h>
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
    char buf[BUFSIZE];

    if (argc != 2) {
        printf("Loony must be launched with 'loony filename'\n");
        return 1;
    }


    if (filebuf_load_file(fbuf, argv[1])) {
        printf("Couldn't load file %s, exiting.\n", argv[1]);
        return 1;
    }

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
        } else if (ch == 'j') {
            filebuf_move_cursor(fbuf, 1, 0);
        } else if (ch == 'k') {
            filebuf_move_cursor(fbuf, -1, 0);
        } else if (ch == 'o') {
            write_new_line(fbuf, fbuf->crow+1);
        }
        /*
        if (buf[0] == 'a') {
            char tmp[BUFSIZE];
            if (sscanf(buf, "%*1s%*1[ ]%[^\n]", tmp) == 1) {
                FileLine *line = fileline_init(tmp);
                filebuf_append_line(fbuf, line);
            }
        } else if (buf[0] == 'i') {
            size_t pos;
            char tmp[BUFSIZE];
            if (sscanf(buf, "%*s %lu%*1[ ]%[^\n]", &pos, tmp) == 2) {
                FileLine *line = fileline_init(tmp);
                filebuf_insert_line(fbuf, line, pos-1);
            }
        } else if (buf[0] == 'd') {
            size_t pos;
            if (sscanf(buf, "%*s %lu", &pos) == 1) {
                filebuf_delete_line(fbuf, pos-1);
            }
        } else if (buf[0] == 'c') {
            size_t pos;
            char tmp[BUFSIZE];
            if (sscanf(buf, "%*s %lu%*1[ ]%[^\n]", &pos, tmp) == 2) {
                FileLine *line = fileline_init(tmp);
                filebuf_replace_line(fbuf, line, pos-1);
            }
        } else if (buf[0] == 'l') {
            size_t first, last;
            if (sscanf(buf, "%*s %lu %lu", &first, &last) == 2) {
                filebuf_print(fbuf, first-1, last-1);
            } else {
                filebuf_print(fbuf, 0, fbuf->num_lines-1);
            }
        } else if (buf[0] == 'w') {
            filebuf_save_file(fbuf, argv[1]);
        } else if (buf[0] == 'r') {
            filebuf_load_file(fbuf, argv[1]);
        } else if (buf[0] == 'q') {
            goto end;
        } else {
            fprintf(stderr, "Invalid command %c\n", buf[0]);
        }
        */
    }

end:
    endwin();
    filebuf_free(fbuf);
    return 0;
}
