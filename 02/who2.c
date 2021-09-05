/**
 * Created by Zhang Shengzhe on 2021/09/02.
 */

#include <stdio.h>
#include <utmpx.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#define SHOW_HOST

void show_info(struct utmpx *utbufp);

void showtime(long timeval);

int main(void)
{
    struct utmpx utbuf;
    int utmpx_fd;

    if ((utmpx_fd = open(_PATH_UTMPX, O_RDONLY)) == -1) {
        perror(_PATH_UTMPX);
        _exit(1);
    }

    while (read(utmpx_fd, &utbuf, sizeof(utbuf)) == sizeof(utbuf))
        show_info(&utbuf);

    close(utmpx_fd);

    return 0;
}

void show_info(struct utmpx *utbufp)
{
    if (utbufp->ut_type != USER_PROCESS)
        return;
    printf(" %-8.8s", utbufp->ut_user);
    printf(" ");
    printf(" %-8.8s", utbufp->ut_line);
    printf(" ");
    showtime(utbufp->ut_tv.tv_sec);
    printf(" ");

#ifdef SHOW_HOST
    printf("(%s)", utbufp->ut_host);
#endif
    printf("\n");
}

void showtime(long timeval)
{
    char *cp;
    cp = ctime(&timeval);

    printf("%12.12s", cp + 4);
}
