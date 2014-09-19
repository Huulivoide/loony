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
} FileBuffer;

/*
 * FileLine functions
 */

/* Creates a new FileLine with the given text (it will be copied).
 * Returns NULL in case of error. */
FileLine *fileline_init (const char *text);

/* Destroys a FileLine. */
void fileline_free (FileLine *line);

/*
 * FileBuffer functions
 */

/* Creates an empty FileBuffer. Returns NULL in case of error. */
FileBuffer *filebuf_init (void);

/* Destroys a FileBuffer. */
void filebuf_free (FileBuffer *buf);

/* Adds a new line at the end of the buffer. Returns 0 on success. */
int filebuf_append_line (FileBuffer *buf, FileLine *line);

/* Replaces a line with a new line. Returns 0 on success. */
int filebuf_replace_line (FileBuffer *buf, FileLine *line, size_t pos);

/* Loads the given file into a FileBuffer. Returns 0 on success. */
int filebuf_load_file (FileBuffer *buf, const char *filename);

/* Saves a FileBuffer into the given file. Returns 0 on success. */
int filebuf_save_file (FileBuffer *buf, const char *filename);

/* Prints lines from buf to stdout. Keep in mind that the first line is 0. */
void filebuf_print (const FileBuffer *buf, size_t first, size_t last);
