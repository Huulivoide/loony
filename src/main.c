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
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include "cursesio.h"
#include "lua_api.h"
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

    /* start Lua */
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    loonyapi_open(L);
    if (luaL_dofile(L, SCRIPTDIR"/commands.lua")) {
        loonywin_set_statusbar(win, lua_tostring(L, -1));
        lua_pop(L, 1);
    }

    char cmd_buf[256];
    size_t cmd_len = 0;
    for (;;) {
        int ch;

        display_win(win);

        ch = getch();
        if (ch == 'q') {
            goto end;
        }
        cmd_buf[cmd_len] = ch;
        ++cmd_len;
        cmd_buf[cmd_len] = '\0';
        loonywin_set_statusbar(win, cmd_buf);

        LoonyApiStatus status = loonyapi_get_callback(L, cmd_buf);
        if (status == LOONYAPI_OK) {
            loonyapi_push_loonywin(L, win);
            int err = lua_pcall(L, 1, 0, 0);
            if (err) {
                loonywin_set_statusbar(win, lua_tostring(L, -1));
                lua_pop(L, 1);
            }
        }

        /*
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
        */
    }

end:
    endwin();
    loonywin_free(win);
    textbuf_free(tbuf);
    return 0;
}
