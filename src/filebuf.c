/*
 * filebuf.c
 *
 * Functions and structs for handling reading from / writing to files.
 */

#include "filebuf.h"

#include <assert.h>
#include <stdio.h>
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

int filebuf_load_file (FileBuffer *buf, const char *filename)
{
    FILE *fp = NULL;
    char *line = NULL;
    size_t n = 0;
    ssize_t num_chars = 0;
    size_t i;

    assert(buf != NULL);
    assert(filename != NULL);

    fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Couldn't open file %s for reading\n", filename);
        return 1;
    }

    for (i = 0; i < buf->num_lines; ++i) {
        fileline_free(buf->lines[i]);
    }
    free(buf->lines);
    buf->linebuf_size = 0;
    buf->num_lines = 0;

    while ((num_chars = getline(&line, &n, fp)) != -1) {
        filebuf_append_line(buf, fileline_init(line));
        free(line);
        line = NULL;
        n = 0;
    }
    free(line);

    fclose(fp);
    return 0;
}

int filebuf_save_file (FileBuffer *buf, const char *filename)
{
    FILE *fp;
    size_t i;

    assert(buf != NULL);
    assert(filename != NULL);

    fp = fopen(filename, "w");
    if (!fp) {
        fprintf(stderr, "Couldn't open file %s for writing\n", filename);
        return 1;
    }

    for (i = 0; i < buf->num_lines; ++i) {
        fprintf(fp, "%s\n", buf->lines[i]->text);
    }

    fclose(fp);
    return 0;
}
