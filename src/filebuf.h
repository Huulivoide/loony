#pragma once

/*
 * filebuf.h
 *
 * Functions and structs for handling reading from / writing to files.
 */

#include <stddef.h>

/*
 * Represents one line in a file.
 */
typedef struct FileLine
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
} FileLine;

/*
 * Represents a complete file.
 */
typedef struct FileBuffer
{
    /* array of lines. TODO: change to a more efficient data structure */
    FileLine **lines;
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
} FileBuffer;

/*
 * FileLine functions
 */

/* Creates a new FileLine with the given text (it will be copied).
 * Returns NULL in case of error. */
FileLine *fileline_init (const char *text);

/* Destroys a FileLine. */
void fileline_free (FileLine *line);

/* Inserts text in a FileLine. Returns 0 on success. */
int fileline_insert (FileLine *line, const char *text, size_t pos);

/* Deletes all text from pos to end of line. Returns 0 on success. */
int fileline_delete_to_eol(FileLine *line, size_t pos);

/*
 * FileBuffer functions
 */

/* Creates an empty FileBuffer. Returns NULL in case of error. */
FileBuffer *filebuf_init (void);

/* Destroys a FileBuffer. */
void filebuf_free (FileBuffer *buf);

/* Adds a new line at the end of the buffer. Returns 0 on success. */
int filebuf_append_line (FileBuffer *buf, FileLine *line);

/* Adds a new line at the given position. Returns 0 on success. */
int filebuf_insert_line (FileBuffer *buf, FileLine *line, size_t pos);

/* Deletes the given line. Returns 0 on success. */
int filebuf_delete_line (FileBuffer *buf, size_t pos);

/* Replaces a line with a new line. Returns 0 on success. */
int filebuf_replace_line (FileBuffer *buf, FileLine *line, size_t pos);

/* Join a line with the following line. Returns 0 on success. */
int filebuf_join_with_next_line(FileBuffer *buf, size_t line);

/* Splits the given line into two; the new line will start from the given
 * position. Returns 0 on success. */
int filebuf_split_line(FileBuffer *buf, size_t line, size_t pos);

/* Loads the given file into a FileBuffer. Returns 0 on success. */
int filebuf_load_file (FileBuffer *buf, const char *filename);

/* Saves a FileBuffer into the given file. Returns 0 on success. */
int filebuf_save_file (FileBuffer *buf, const char *filename);

/* Moves cursor dy rows down and dx columns to the right.
 * INT_MIN moves the cursor to the first line or column;
 * INT_MAX moves the cursor to the last line or column. */
void filebuf_move_cursor (FileBuffer *buf, int dy, int dx);

/* Returns the current line / column. */
int filebuf_current_line(FileBuffer *buf);
int filebuf_current_col(FileBuffer *buf);

/* Deletes the character under the cursor. Returns 0 on success. */
int filebuf_delete_char (FileBuffer *buf);
