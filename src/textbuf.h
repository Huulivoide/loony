#pragma once

/*
 * textbuf.h
 *
 * A text buffer is basically equivalent to a single file.
 */

#include <stddef.h>

/*
 * Represents one line of text.
 */
typedef struct TextLine
{
    /* the text of the line, excluding the final newline */
    char *text;
    /* size of the text array */
    size_t textbuf_size;
    /* number of characters in the text array */
    size_t num_chars;
    /* number of bytes in the text array (remember that UTF-8 characters can be
     * more than one byte */
    size_t num_bytes;
} TextLine;

/*
 * Represents a complete file.
 */
typedef struct TextBuffer
{
    /* array of lines. TODO: change to a more efficient data structure */
    TextLine **lines;
    /* size of the lines array */
    size_t linebuf_size;
    /* number of lines in the buffer */
    size_t num_lines;
    /* cursor location. first row/column is number 0. */
    int crow, ccol;
    /* first row displayed on screen */
    int firstrow;
    /* non-zero if the screen should be completely redrawn */
    int redraw_needed;
} TextBuffer;

/*
 * TextLine functions
 */

/* Creates a new TextLine with the given text (it will be copied).
 * Returns NULL in case of error. */
TextLine *textline_init(const char *text);

/* Destroys a TextLine. */
void textline_free(TextLine *line);

/* Inserts text in a TextLine. Returns 0 on success. */
int textline_insert(TextLine *line, const char *text, size_t pos);

/* Deletes all text from pos to end of line. Returns 0 on success. */
int textline_delete_to_eol(TextLine *line, size_t pos);

/*
 * TextBuffer functions
 */

/* Creates an empty TextBuffer. Returns NULL in case of error. */
TextBuffer *textbuf_init(void);

/* Destroys a TextBuffer. */
void textbuf_free(TextBuffer *buf);

/* Adds a new line at the end of the buffer. Returns 0 on success. */
int textbuf_append_line(TextBuffer *buf, TextLine *line);

/* Adds a new line at the given position. Returns 0 on success. */
int textbuf_insert_line(TextBuffer *buf, TextLine *line, size_t pos);

/* Inserts text at the cursor and moves the cursor past the new text.
 * Returns 0 on success. */
int textbuf_insert_at_cursor(TextBuffer *buf, const char *text);

/* Deletes the given line. Returns 0 on success. */
int textbuf_delete_line(TextBuffer *buf, size_t pos);

/* Replaces a line with a new line. Returns 0 on success. */
int textbuf_replace_line(TextBuffer *buf, TextLine *line, size_t pos);

/* Join a line with the following line. Returns 0 on success. */
int textbuf_join_with_next_line(TextBuffer *buf, size_t line);

/* Splits the given line into two; the new line will start from the given
 * position. Returns 0 on success. */
int textbuf_split_line(TextBuffer *buf, size_t line, size_t pos);

/* Loads the given file into a TextBuffer. Returns 0 on success. */
int textbuf_load_file(TextBuffer *buf, const char *filename);

/* Saves a TextBuffer into the given file. Returns 0 on success. */
int textbuf_save_file(TextBuffer *buf, const char *filename);

/* Moves cursor dy rows down and dx columns to the right.
 * INT_MIN moves the cursor to the first line or column;
 * INT_MAX moves the cursor to the last line or column. */
void textbuf_move_cursor(TextBuffer *buf, int dy, int dx);

/* Returns the current line / column number. */
int textbuf_line_num(TextBuffer *buf);
int textbuf_col_num(TextBuffer *buf);

/* Deletes the character under the cursor. Returns 0 on success. */
int textbuf_delete_char(TextBuffer *buf);
