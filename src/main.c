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
        } else if (buf[0] == 'l') {
            size_t i;
            for (i = 0; i < fbuf->num_lines; ++i) {
                printf("%s\n", fbuf->lines[i]->text);
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
