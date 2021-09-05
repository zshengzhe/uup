/**
 * who1.c - a first version of the who program
 *          open, read UTMP file, and show results
 * Created by Linus Zhang on 2021/9/1.
 */

#include <stdio.h>
#include <stdlib.h>
#include <utmpx.h>
#include <fcntl.h>
#include <unistd.h>

#define SHOWHOST

void show_info(struct utmpx *utbufp);

int main()
{
    struct utmpx current_record;
    int utmpfd;
    int reclen = sizeof(struct utmpx);

    if ((utmpfd = open(UTMPX_FILE, O_RDONLY)) == -1) {
        perror(UTMPX_FILE);
        exit(1);
    }

    while ((read(utmpfd, &current_record, reclen)) == reclen)
        show_info(&current_record);
    close(utmpfd);
    return 0;
}

void show_info(struct utmpx *utbufp)
{
    printf("%-8.8s", utbufp->ut_user);
    printf(" ");
    printf("%-8.8s", utbufp->ut_line);
    printf(" ");
    printf("%101ld", utbufp->ut_tv.tv_sec);
    printf(" ");

#ifdef SHOWHOST
    printf("(%s)", utbufp->ut_host);
#endif
    printf("\n");
}