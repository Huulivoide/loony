/*
 * filebuf.c
 *
 * Functions and structs for handling reading from / writing to files.
 */

#include "filebuf.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

FileLine *fileline_init (const char *text)
{
    FileLine *line;
    size_t num_chars;
    size_t buf_size;
    char *newline;

    assert(text != NULL);

    line = malloc(sizeof(*line));
    if (!line) {
        return NULL;
    }

    num_chars = strlen(text);
    buf_size = 16;
    while (buf_size < num_chars+1) {
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
    line->num_chars = num_chars;
    return line;
}

void fileline_free (FileLine *line)
{
    if (!line) {
        return;
    }

    free(line->text);
    free(line);
}

FileBuffer *filebuf_init (void)
{
    FileBuffer *buf = malloc(sizeof(*buf));
    if (!buf) {
        return NULL;
    }

    buf->lines = NULL;
    buf->linebuf_size = 0;
    buf->num_lines = 0;

    return buf;
}

void filebuf_free (FileBuffer *buf)
{
    size_t i;
    if (!buf) {
        return;
    }

    for (i = 0; i < buf->num_lines; ++i) {
        fileline_free(buf->lines[i]);
    }
    free(buf->lines);
    free(buf);
}

int filebuf_append_line (FileBuffer *buf, FileLine *line)
{
    assert(buf != NULL);
    assert(line != NULL);

    if (buf->num_lines == buf->linebuf_size) {
        if (buf->linebuf_size == 0) {
            buf->linebuf_size = 1;
        } else {
            buf->linebuf_size *= 2;
        }
        buf->lines = realloc(buf->lines, sizeof(FileLine*) * buf->linebuf_size);
    }

    buf->lines[buf->num_lines] = line;
    buf->num_lines += 1;
    return 0;
}
