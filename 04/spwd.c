/**
 * spwd.c: a simplified version of pwd
 *
 * starts in current directory and recursively
 * climbs up to rrot of filesystem, prints top part
 * then prints current part
 *
 * uses readdir() to get info about each thing
 *
 * ERROR
 * Created by Linus Zhang on 2021/9/5.
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>

ino_t get_inode(char *fname);

void printpathto(ino_t this_inode);

void inum_to_name(ino_t inode_to_find, char *name_buf, int buflen);

int main()
{
    printpathto(get_inode("."));
    putchar('\n');
    return 0;
}

/**
 * prints path leading down to an object with this inode
 * kind of recursive
 * @param this_inode
 */
void printpathto(ino_t this_inode)
{
    ino_t my_inode;
    char its_name[BUFSIZ];
    static char path[BUFSIZ] = "";
    if (get_inode("..") != this_inode) {
        chdir("..");
        inum_to_name(this_inode, its_name, BUFSIZ);
        strcat(path, "/");
        strcat(path, its_name);
        my_inode = get_inode(".");
        printpathto(my_inode);
        printf("/%s", its_name);
    }
}

/**
 * looks through current directory for a file with this inode
 * number and copies its name into name_buf
 * @param inode_to_find
 * @param name_buf
 * @param buflen
 */
void inum_to_name(ino_t inode_to_find, char *name_buf, int buflen)
{
    DIR *dir_ptr;
    struct dirent *direntp;

    dir_ptr = opendir(".");
    if (dir_ptr == NULL) {
        perror(".");
        _exit(1);
    }

    // search directory for a file with specified inum
    while ((direntp = readdir(dir_ptr)) != NULL) {
        if (direntp->d_ino == inode_to_find) {
            strncpy(name_buf, direntp->d_name, buflen);
            name_buf[buflen - 1] = '\0';
            closedir(dir_ptr);
            return;
        }
    }
    fprintf(stderr, "error looking for inum %lld\n", inode_to_find);
    _exit(1);
}

/**
 * returns inode number of the file
 * @param fname
 * @return
 */
ino_t get_inode(char *fname)
{
    struct stat info;

    if (stat(fname, &info) == -1) {
        fprintf(stderr, "Cannot stat ");
        perror(fname);
        _exit(1);
    }
    return info.st_ino;
}

