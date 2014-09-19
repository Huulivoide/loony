/*
 * main.c
 *
 * This file contains the main() function and other stuff used at startup.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void save_buf (const char *filename, const char *buf)
{
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        fprintf(stderr, "Couldn't open file %s for writing", filename);
        return;
    }
    while (*buf) {
        putc(*buf, fp);
        buf++;
    }
    fclose(fp);
}

int main (int argc, char *argv[])
{
#define BUFSIZE 1024
    char fbuf[BUFSIZE];
    memset(fbuf, 0, BUFSIZE);
    size_t used_fbuf = 1; /* NUL */
    char buf[BUFSIZE];

    if (argc != 2) {
        printf("Loony must be launched with 'loony filename'\n");
        return 1;
    }

    while (fgets(buf, BUFSIZE, stdin)) {
        switch (buf[0]) {
            case 'a':
                strncat(fbuf, buf+1, BUFSIZE-used_fbuf);
                used_fbuf += strlen(buf+1);
                break;
            case 'l':
                printf("%s", fbuf);
                break;
            case 'w':
                save_buf(argv[1], fbuf);
                break;
            case 'q':
                exit(0);
                break;
            default:
                fprintf(stderr, "Invalid command %c", *buf);
                break;
        }
    }
}
