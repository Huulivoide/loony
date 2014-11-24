#include "window.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

LoonyWindow *loonywin_init(TextBuffer *buf, WINDOW *win)
{
    LoonyWindow *window;

    assert(buf != NULL);
    assert(win != NULL);

    window = malloc(sizeof(LoonyWindow));

    if (!window) {
        return NULL;
    }

    window->buffer = buf;
    window->window = win;
    window->firstrow = 0;
    window->redraw_needed = 0;
    window->statusbar_text[0] = '\0';

    return window;
}

void loonywin_free(LoonyWindow *win)
{
    free(win);
}

void loonywin_move_cursor(LoonyWindow *win, int dy, int dx)
{
    size_t win_h, win_w;

    assert(win != NULL);
    assert(win->buffer != NULL);

    if (dx == 0 && dy == 0) {
        return;
    }

    getmaxyx(win->window, win_h, win_w);
    --win_h; /* save a line for the statusbar */

    textbuf_move_cursor(win->buffer, dy, dx);

    /* scrolling required? */
    if (win->buffer->crow < win->firstrow) {
        win->firstrow = win->buffer->crow;
    } else if (win->buffer->crow >= win->firstrow + win_h) {
        win->firstrow = win->buffer->crow - win_h + 1;
    }
}

void loonywin_set_statusbar(LoonyWindow *win, const char *text)
{
    strncpy(win->statusbar_text, text, STATUSBAR_LENGTH - 1);
    win->statusbar_text[STATUSBAR_LENGTH-1] = '\0';
}

TextBuffer *loonywin_get_buffer(LoonyWindow *win)
{
    return win->buffer;
}
