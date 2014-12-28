/*
 * textbuf.c
 */

#include "textbuf.h"

#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <curses.h>

#include "util.h"

TextLine *textline_init(const char *text)
{
    TextLine *line;
    size_t num_bytes;
    size_t buf_size;
    char *newline;

    assert(text != NULL);

    line = malloc(sizeof(*line));
    if (!line) {
        return NULL;
    }

    num_bytes = strlen(text);
    buf_size = 16;
    while (buf_size < num_bytes+1) {
        buf_size *= 2;
    }

    line->text = malloc(buf_size);
    if (!line->text) {
        free(line);
        return NULL;
    }

    strcpy(line->text, text);
    if ((newline = strchr(line->text, '\n'))) {
        *newline = '\0';
    }
    line->textbuf_size = buf_size;
    line->num_chars = u8strlen(text);
    line->num_bytes = num_bytes;
    line->prev = NULL;
    line->next = NULL;
    return line;
}

void textline_free(TextLine *line)
{
    if (!line) {
        return;
    }

    free(line->text);
    free(line);
}

int textline_insert(TextLine *line, const char *text, size_t pos)
{
    size_t num_bytes = strlen(text);
    size_t new_size = line->num_bytes + num_bytes + 1;
    size_t u8pos;
    char *tmp;

    if (pos == line->num_chars) {
        if (line->textbuf_size < new_size) {
            while (line->textbuf_size < new_size) {
                line->textbuf_size *= 2;
            }
            line->text = realloc(line->text, line->textbuf_size);
        }
        strcat(line->text, text);
        goto update_size;
    }

    if (u8_find_pos(line->text, pos, &u8pos)) {
        return 1;
    }

    tmp = malloc(line->num_bytes - u8pos + 1);

    if (line->textbuf_size < new_size) {
        while (line->textbuf_size < new_size) {
            line->textbuf_size *= 2;
        }
        line->text = realloc(line->text, line->textbuf_size);
    }

    strcpy(tmp, line->text + u8pos);
    strcpy(line->text + u8pos, text);
    strcat(line->text, tmp);
    free(tmp);
update_size:
    line->num_bytes = new_size - 1; /* don't count the '\0' */
    line->num_chars = u8strlen(line->text);

    return 0;
}

int textline_delete_to_eol(TextLine *line, size_t pos)
{
    size_t u8pos;

    if (pos > line->num_chars) {
        return 1;
    }

    if (u8_find_pos(line->text, pos, &u8pos)) {
        return 1;
    }

    line->text[u8pos] = '\0';
    line->num_bytes = u8pos;
    line->num_chars = pos;
    return 0;
}

/*
 * Internal functions to simplify some tasks
 */

/* Returns a pointer to the given TextLine. */
static TextLine *textbuf_get_textline(const TextBuffer *buf, size_t pos)
{
    TextLine *tmp;
    assert(buf != NULL);

    tmp = buf->head;
    while (pos-- > 0) {
        assert(tmp != NULL);
        tmp = tmp->next;
    }

    return tmp;
}

TextBuffer *textbuf_init(void)
{
    TextBuffer *buf = malloc(sizeof(*buf));
    if (!buf) {
        return NULL;
    }

    buf->head = NULL;
    buf->tail = NULL;
    buf->num_lines = 0;
    buf->crow = 0;
    buf->ccol = 0;

    /* a buffer should always have at least one line */
    textbuf_append_line(buf, textline_init(""));

    return buf;
}

void textbuf_free(TextBuffer *buf)
{
    TextLine *tmp;
    if (!buf) {
        return;
    }

    tmp = buf->head;
    while (tmp) {
        TextLine *next = tmp->next;
        textline_free(tmp);
        tmp = next;
    }

    free(buf);
}

int textbuf_append_line(TextBuffer *buf, TextLine *line)
{
    assert(buf != NULL);
    assert(line != NULL);

    if (buf->tail) {
        buf->tail->next = line;
    } else {
        buf->head = line;
    }
    line->prev = buf->tail;
    line->next = NULL;
    buf->tail = line;

    buf->num_lines += 1;
    return 0;
}

int textbuf_insert_line(TextBuffer *buf, TextLine *line, size_t pos)
{
    assert(buf != NULL);
    assert(line != NULL);

    if (buf->num_lines < pos) {
        fprintf(stderr,
                "Can't insert before line %zu because it doesn't exist!\n",
                pos+1);
        return 1;
    }

    if (buf->head == NULL) {
        buf->head = line;
        buf->tail = line;
        line->prev = NULL;
        line->next = NULL;
    } else if (pos == buf->num_lines) {
        return textbuf_append_line(buf, line);
    } else {
        TextLine *tmp = textbuf_get_textline(buf, pos);

        /* this should've been handled in the previous if statement unless
         * num_lines is incorrect */
        assert(tmp != NULL);

        if (tmp->prev) {
            tmp->prev->next = line;
        } else {
            buf->head = line;
        }
        line->prev = tmp->prev;
        line->next = tmp;
        tmp->prev = line;
    }

    buf->num_lines += 1;
    return 0;
}

int textbuf_insert_at_cursor(TextBuffer *buf, const char *text)
{
    int err;
    if ((err = textline_insert(textbuf_get_textline(buf, buf->crow),
                               text, buf->ccol))) {
        return err;
    }
    textbuf_move_cursor(buf, 0, u8strlen(text));
}

int textbuf_delete_line(TextBuffer *buf, size_t pos)
{
    TextLine *tmp;

    assert(buf != NULL);

    if (buf->num_lines <= pos) {
        fprintf(stderr,
                "Can't delete line %zu because it doesn't exist!\n", pos+1);
        return 1;
    }

    tmp = textbuf_get_textline(buf, pos);

    if (tmp->prev) {
        tmp->prev->next = tmp->next;
    } else {
        buf->head = tmp->next;
    }

    if (tmp->next) {
        tmp->next->prev = tmp->prev;
    } else {
        buf->tail = tmp->prev;
    }

    textline_free(tmp);
    buf->num_lines -= 1;

    if (buf->crow == buf->num_lines) {
        textbuf_move_cursor(buf, -1, 0);
    }

    /* If the next line is shorter than the deleted one, the cursor may be past
     * its end. Move the cursor if that is the case. */
    tmp = textbuf_get_textline(buf, textbuf_line_num(buf));
    if (textbuf_col_num(buf) >= tmp->num_chars) {
        textbuf_move_cursor(buf, 0, INT_MAX);
    }

    /* make sure there's always at least one line in the buffer */
    if (buf->num_lines == 0) {
        textbuf_append_line(buf, textline_init(""));
        textbuf_move_cursor(buf, INT_MIN, INT_MIN);
    }

    return 0;
}

int textbuf_replace_line(TextBuffer *buf, TextLine *line, size_t pos)
{
    TextLine *tmp;

    assert(buf != NULL);
    assert(line != NULL);

    if (buf->num_lines <= pos) {
        fprintf(stderr,
                "Can't replace line %zu because it doesn't exist!\n", pos+1);
        return 1;
    }

    tmp = textbuf_get_textline(buf, pos);

    if (tmp->prev) {
        tmp->prev->next = line;
    } else {
        buf->head = line;
    }

    if (tmp->next) {
        tmp->next->prev = line;
    } else {
        buf->tail = line;
    }

    textline_free(tmp);
    return 0;
}

int textbuf_join_with_next_line(TextBuffer *buf, size_t pos)
{
    int old_num_chars;
    TextLine *tmp;
    TextLine *next;
    if (pos >= (buf->num_lines - 1)) {
        return 1;
    }

    tmp = textbuf_get_textline(buf, pos);
    next = tmp->next;
    old_num_chars = tmp->num_chars;
    textline_insert(tmp, next->text, tmp->num_chars);
    if (next->next) {
        next->next->prev = tmp;
    } else {
        buf->tail = tmp;
    }
    tmp->next = next->next;
    textline_free(next);
    buf->crow = pos;
    buf->ccol = old_num_chars;
    --buf->num_lines;
    return 0;
}

int textbuf_split_line(TextBuffer *buf, size_t line, size_t pos)
{
    size_t u8pos;
    TextLine *tmp;
    
    tmp = textbuf_get_textline(buf, line);
    if (line >= buf->num_lines || pos > tmp->num_chars) {
        return 1;
    }

    if (pos == tmp->num_bytes) {
        return textbuf_insert_line(buf, textline_init(""), line+1);
    } else {
        if (u8_find_pos(tmp->text, pos, &u8pos)) {
            return 1;
        }

        if (textbuf_insert_line(buf, textline_init(tmp->text+u8pos), line+1)) {
            return 1;
        }

        if (textline_delete_to_eol(tmp, pos)) {
            return 1;
        }
    }

    return 0;
}

int textbuf_load_file(TextBuffer *buf, const char *filename)
{
    FILE *fp = NULL;
    char *line = NULL;
    size_t n = 0;
    ssize_t num_chars = 0;
    TextLine *tmp;

    assert(buf != NULL);
    assert(filename != NULL);

    fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Couldn't open file %s for reading\n", filename);
        return 1;
    }

    tmp = buf->head;
    while (tmp) {
        TextLine *next = tmp->next;
        textline_free(tmp);
        tmp = next;
    }
    buf->head = NULL;
    buf->tail = NULL;
    buf->num_lines = 0;

    while ((num_chars = getline(&line, &n, fp)) != -1) {
        char *newline;
        if ((newline = strchr(line, '\n'))) {
            *newline = '\0';
        }
        textbuf_append_line(buf, textline_init(line));
        free(line);
        line = NULL;
        n = 0;
    }
    free(line);

    fclose(fp);
    return 0;
}

int textbuf_save_file(TextBuffer *buf, const char *filename)
{
    FILE *fp;
    TextLine *tmp;

    assert(buf != NULL);
    assert(filename != NULL);

    fp = fopen(filename, "w");
    if (!fp) {
        fprintf(stderr, "Couldn't open file %s for writing\n", filename);
        return 1;
    }

    tmp = buf->head;
    while (tmp) {
        fprintf(fp, "%s\n", tmp->text);
        tmp = tmp->next;
    }

    fclose(fp);
    return 0;
}

void textbuf_move_cursor(TextBuffer *buf, int dy, int dx)
{
    if (dx == 0 && dy == 0) {
        return;
    }

    assert(buf != NULL);

    if (dy == INT_MIN) {
        buf->crow = 0;
    } else if (dy == INT_MAX) {
        buf->crow = buf->num_lines-1;
    } else {
        buf->crow += dy;
    }

    if (dx == INT_MIN) {
        buf->ccol = 0;
    } else if (dx == INT_MAX) {
        buf->ccol = textbuf_get_textline(buf, buf->crow)->num_chars;
    } else {
        buf->ccol += dx;
    }

    if (buf->crow < 0) {
        buf->crow = 0;
    } else if (buf->crow >= buf->num_lines) {
        buf->crow = buf->num_lines - 1;
    }

    if (buf->ccol < 0) {
        buf->ccol = 0;
    } else if (buf->ccol > textbuf_get_textline(buf, buf->crow)->num_chars) {
        buf->ccol = textbuf_get_textline(buf, buf->crow)->num_chars;
    }
}

int textbuf_line_num(TextBuffer *buf)
{
    return buf->crow;
}

int textbuf_col_num(TextBuffer *buf)
{
    return buf->ccol;
}

int textbuf_delete_char(TextBuffer *buf)
{
    /* First byte that should be deleted. Remember, UTF-8 characters can be
     * more than one byte! */
    size_t first_to_delete;
    /* How many bytes must be deleted? */
    size_t deleted_bytes;
    size_t i;
    TextLine *tmp;

    assert(buf != NULL);

    tmp = textbuf_get_textline(buf, buf->crow);
    if (!tmp) {
        return 1;
    }

    if (buf->ccol >= tmp->num_chars) {
        return 0; /* cursor is one character past the end of the line */
    }

    if (u8_find_pos(tmp->text, buf->ccol, &first_to_delete)) {
        return 1;
    }

    deleted_bytes = u8_char_length(tmp->text[first_to_delete]);

    if (deleted_bytes == -1) {
        return 1;
    }

    for (i = first_to_delete; i <= tmp->num_bytes - deleted_bytes; ++i) {
        tmp->text[i] = tmp->text[i+deleted_bytes];
    }

    tmp->num_bytes -= deleted_bytes;
    tmp->num_chars -= 1;
    return 0;
}

const char *textbuf_get_line(const TextBuffer *buf, size_t line)
{
    if (buf->num_lines <= line) {
        return NULL;
    } else {
        TextLine *tmp = textbuf_get_textline(buf, line);
        if (!tmp) {
            return NULL;
        } else {
            return tmp->text;
        }
    }
}

const char *textbuf_current_line(const TextBuffer *buf)
{
    TextLine *tmp = textbuf_get_textline(buf, buf->crow);
    if (!tmp) {
        return NULL;
    } else {
        return tmp->text;
    }
}
