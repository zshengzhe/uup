/**
 * cp1.c
 * version 1 of cp - uses read and write with tunable buffer size
 * Created by Linus Zhang on 2021/9/1.
 */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 4096
#define COPY_MODE 0644

void oops(char *, char *);

int main(int argc, char *argv[])
{
    int in_fd, out_fd, n_chars;
    char buf[BUFFER_SIZE];

    if (argc != 3) {
        fprintf(stderr, "usage: %s source destination\n", *argv);
        _exit(1);
    }

    if ((in_fd = open(argv[1], O_RDONLY)) == -1)
        oops("Cannot open", argv[1]);

    if ((out_fd = creat(argv[2], COPY_MODE)) == -1)
        oops("Cannot creat ", argv[2]);

    while ((n_chars = (int) read(in_fd, buf, BUFFER_SIZE)) > 0) {
        if (write(out_fd, buf, n_chars) != n_chars)
            oops("Write error to ", argv[2]);
    }
    if (n_chars == -1)
        oops("Read error from ", argv[1]);

    if (close(in_fd) == -1 || close(out_fd) == -1)
        oops("Error closing files", "");
}

void oops(char *s1, char *s2)
{
    fprintf(stderr, "Error: %s", s1);
    perror(s2);
    _exit(1);
}