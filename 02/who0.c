/**
 * Created by Zhang Shengzhe on 2021/09/02.
 */

#include <stdio.h>
#include <utmpx.h>

#define SHOW_HOST

void show_info(struct utmpx *utbufp);

void show_time(long timeval);

int main(void)
{
    struct utmpx *utp;
    while ((utp = getutxent()) != NULL)
        show_info(utp);

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
    show_time(utbufp->ut_tv.tv_sec);
    printf(" ");

#ifdef SHOW_HOST
    printf("(%s)", utbufp->ut_host);
#endif
    printf("\n");
}

void show_time(long timeval)
{
    char *p;
    p = ctime(&timeval);
    printf("%12.12s", p + 4);
}