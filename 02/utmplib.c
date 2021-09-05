/**
 * Created by Zhang Shengzhe on 2021/09/03.
 */

#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <utmpx.h>
#include <unistd.h>

#define NRECS   16
#define NULLUT  ((struct utmpx *)NULL)
#define UTSIZE  (sizeof(struct utmpx))

int utmpx_reload();

void utmpx_close();

/* storage      */
static char utmpxbuf[NRECS * UTSIZE];
/* num stored   */
static int num_recs;
/* next to go   */
static int cur_rec;
/* read from    */
static int fd_utmpx = -1;

int utmpx_open(char *filename)
{
    /* open it      */
    fd_utmpx = open(filename, O_RDONLY);
    /* no recs yet  */
    cur_rec = num_recs = 0;
    /* report       */
    return fd_utmpx;
}

struct utmpx *utmpx_next()
{
    struct utmpx *recp;

    /* error ?      */
    if (fd_utmpx == -1)
        return NULLUT;
    /* any more ?   */
    if (cur_rec == num_recs && utmpx_reload() == 0)
        return NULLUT;
    /* get address of next record    */
    recp = (struct utmpx *) &utmpxbuf[cur_rec * UTSIZE];
    cur_rec++;
    return recp;
}

/*
 *      read next bunch of records into buffer
 */
int utmpx_reload()
{
    int amt_read;

    /* read them in         */
    amt_read = read(fd_utmpx, utmpxbuf, NRECS * UTSIZE);

    /* how many did we get? */
    num_recs = amt_read / UTSIZE;
    /* reset pointer        */
    cur_rec = 0;
    return num_recs;
}

void utmpx_close()
{
    /* don't close if not   */
    if (fd_utmpx != -1)
        close(fd_utmpx);
}