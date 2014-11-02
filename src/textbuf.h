/**
 * @file textbuf.h
 *
 * TextBuffers are the main structures of Loony. They are basically equivalent
 * to single files.
 *
 * Many of the functions here take indices as a parameter. It's important to
 * note that these indices always refer to characters, never to bytes. The
 * functions calculate the correct bytes automatically.
 */
#pragma once

#include <stddef.h>

/** maximum length of statusbar text */
#define STATUSBAR_LENGTH 256

/**
 * Represents one line of text.
 */
typedef struct TextLine
{
    /** the text of the line, excluding the final newline */
    char *text;
    /** size of the text array */
    size_t textbuf_size;
    /** number of characters in the text array */
    size_t num_chars;
    /**
     * Number of bytes in the text array. Not the same as the number of
     * characters if there are any multibyte characters.
     */
    size_t num_bytes;
    /** previous line in the buffer */
    struct TextLine *prev;
    /** next line in the buffer */
    struct TextLine *next;
} TextLine;

/**
 * Represents a complete file.
 */
typedef struct TextBuffer
{
    /** first line in the buffer */
    TextLine *head;
    /** last line in the buffer */
    TextLine *tail;
    /** number of lines in the buffer */
    size_t num_lines;
    /** row number of cursor (first row is 0) */
    int crow;
    /** column number of cursor (first column is 0) */
    int ccol;
    /** first row displayed on screen */
    int firstrow;
    /** non-zero if the screen should be completely redrawn */
    int redraw_needed;
    /** text on the statusbar */
    char statusbar_text[STATUSBAR_LENGTH];
} TextBuffer;

/*
 * TextLine functions
 */

/**
 * Creates a new TextLine with the given text.
 *
 * @param text the text on the line (it will be copied)
 * @return pointer to a dynamically allocated TextLine, or NULL in case of error
 */
TextLine *textline_init(const char *text);

/**
 * Destroys a TextLine.
 *
 * @param line the Textline to be destroyed
 */
void textline_free(TextLine *line);

/**
 * Inserts text in a TextLine.
 *
 * @param line
 * @param text the text to be inserted (will be copied)
 * @param pos The position where the new text should be inserted. pos will be
 * the index where the new text will begin. It must be in the range [0, n],
 * where n is the number of characters on the line.
 * @return 0 on success, non-zero otherwise
 */
int textline_insert(TextLine *line, const char *text, size_t pos);

/**
 * Deletes all text starting from the given position.
 *
 * @param line
 * @param pos The first character to be deleted. It must be in the range
 * [0, n[, where n is the number of characters on the line.
 * @return 0 on success, non-zero otherwise
 */
int textline_delete_to_eol(TextLine *line, size_t pos);

/*
 * TextBuffer functions
 */

/* Creates an empty TextBuffer. Returns NULL in case of error. */
/**
 * Creates an empty TextBuffer.
 *
 * @return pointer to a dynamically allocated Textbuffer, or NULL if there was
 * an error
 */
TextBuffer *textbuf_init(void);

/**
 * Destroys a TextBuffer.
 *
 * @param buf
 */
void textbuf_free(TextBuffer *buf);

/**
 * Adds a new line at the end of a TextBuffer.
 *
 * @param buf
 * @param line the line to be added (it won't be copied)
 * @return 0 on success, non-zero otherwise
 */
int textbuf_append_line(TextBuffer *buf, TextLine *line);

/**
 * Adds a new line at the given position.
 *
 * @param buf
 * @param line the line to be added (it won't be copied)
 * @param pos The index of the new line. It must be in the range [0, n], where
 * n is the number of lines in the buffer.
 * @return 0 on success, non-zero otherwise
 */
int textbuf_insert_line(TextBuffer *buf, TextLine *line, size_t pos);

/**
 * Inserts text at the cursor and moves the cursor past the new text.
 *
 * @param buf
 * @param text the text to be inserted (it will be copied)
 * @return 0 on success, non-zero otherwise
 */
int textbuf_insert_at_cursor(TextBuffer *buf, const char *text);

/**
 * Deletes a line from a buffer.
 *
 * @param buf
 * @param pos the index of the line to be deleted
 * @return 0 on success, non-zero otherwise
 */
int textbuf_delete_line(TextBuffer *buf, size_t pos);

/**
 * Replaces a line with another line. The old line will be deleted.
 *
 * @param buf
 * @param line the new line (it will be copied)
 * @param pos index of the line to be replaced
 * @return 0 on success, non-zero otherwise
 */
int textbuf_replace_line(TextBuffer *buf, TextLine *line, size_t pos);

/**
 * Joins a line with the following line.
 *
 * @param buf
 * @param line index of the line to join with the next line
 * @return 0 on success, non-zero otherwise
 */
int textbuf_join_with_next_line(TextBuffer *buf, size_t line);

/**
 * Splits a line into two lines.
 *
 * @param buf
 * @param line index of the line to split
 * @param pos index of the first character of the second line
 * @return 0 on success, non-zero otherwise
 */
int textbuf_split_line(TextBuffer *buf, size_t line, size_t pos);

/**
 * Loads a file into a TextBuffer.
 *
 * @param buf
 * @param filename name of the file to load
 * @return 0 on success, non-zero otherwise
 */
int textbuf_load_file(TextBuffer *buf, const char *filename);

/**
 * Saves a TextBuffer into a file.
 *
 * @param buf
 * @param filename name of the file to save
 * @return 0 on success, non-zero otherwise
 */
int textbuf_save_file(TextBuffer *buf, const char *filename);

/**
 * Moves the cursor in a buffer.
 *
 * @param buf
 * @param dy Vertical movement. Negative numbers move up, positive numbers move
 * down. INT_MIN or INT_MAX move the cursor to the first or last line
 * respectively.
 * @param dx Horizontal movement. Similar to dy: negative numbers move left,
 * positive numbers move right and INT_MIN and INT_MAX move to first or last
 * column. The last column is past the last character on the line.
 */
void textbuf_move_cursor(TextBuffer *buf, int dy, int dx);

/**
 * Returns the current line number.
 *
 * @param buf
 * @return line number
 */
int textbuf_line_num(TextBuffer *buf);

/**
 * Returns the current column number.
 *
 * @param buf
 * @return column number
 */
int textbuf_col_num(TextBuffer *buf);

/**
 * Deletes the character under the cursor.
 *
 * @param buf
 * @return 0 on success, non-zero otherwise
 */
int textbuf_delete_char(TextBuffer *buf);

/**
 * Returns a pointer to a char array that represents the given line.
 *
 * @param buf
 * @param line line number
 * @return pointer to char array or NULL if there is an error
 */
const char *textbuf_get_line(const TextBuffer *buf, size_t line);

/**
 * Returns a pointer to a char array that represents the current line.
 *
 * @param buf
 * @return pointer to char array or NULL if there is an error
 */
const char *textbuf_current_line(const TextBuffer *buf);

/**
 * Sets the text on the statusbar.
 *
 * @param buf
 * @param text the new text
 */
void textbuf_set_statusbar(TextBuffer *buf, const char *text);
