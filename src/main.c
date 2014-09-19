/*
 * main.c
 *
 * This file contains the main() function and other stuff used at startup.
 */

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "filebuf.h"

void save_buf (const char *filename, const FileBuffer *buf)
{
    FILE *fp;
    size_t i;

    fp = fopen(filename, "w");
    if (!fp) {
        fprintf(stderr, "Couldn't open file %s for writing", filename);
        return;
    }

    for (i = 0; i < buf->num_lines; ++i) {
        fprintf(fp, "%s\n", buf->lines[i]->text);
    }

    fclose(fp);
}

int main (int argc, char *argv[])
{
#define BUFSIZE 1024
    FileBuffer *fbuf = filebuf_init();
    char buf[BUFSIZE];

    if (argc != 2) {
        printf("Loony must be launched with 'loony filename'\n");
        return 1;
    }

    while (fgets(buf, BUFSIZE, stdin)) {
        if (buf[0] == 'a') {
            char *tmp = &buf[1];
            while (*tmp && isspace(*tmp)) {
                tmp++;
            }
            if (*tmp) {
                FileLine *line = fileline_init(tmp);
                filebuf_append_line(fbuf, line);
            }
        } else if (buf[0] == 'l') {
            size_t i;
            for (i = 0; i < fbuf->num_lines; ++i) {
                printf("%s\n", fbuf->lines[i]->text);
            }
        } else if (buf[0] == 'w') {
            save_buf(argv[1], fbuf);
        } else if (buf[0] == 'q') {
            goto end;
        } else {
            fprintf(stderr, "Invalid command %c\n", buf[0]);
        }
    }

end:
    filebuf_free(fbuf);
    return 0;
}
