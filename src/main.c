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
        } else if (buf[0] == 'c') {
            size_t pos;
            char *tmp = &buf[1];
            if (sscanf(buf, "%*s %lu %s", &pos, tmp) == 2) {
                FileLine *line = fileline_init(tmp);
                filebuf_replace_line(fbuf, line, pos-1);
            }
        } else if (buf[0] == 'l') {
            size_t first, last;
            if (sscanf(buf, "%*s %lu %lu", &first, &last) == 2) {
                filebuf_print(fbuf, first-1, last-1);
            } else {
                filebuf_print(fbuf, 0, fbuf->num_lines-1);
            }
        } else if (buf[0] == 'w') {
            filebuf_save_file(fbuf, argv[1]);
        } else if (buf[0] == 'r') {
            filebuf_load_file(fbuf, argv[1]);
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
