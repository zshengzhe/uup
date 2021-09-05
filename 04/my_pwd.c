/**
 * Created by Linus Zhang on 2021/9/5.
 */

#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>

ino_t get_inode(char *file_name);

void get_path(char *result_buf, ino_t this_inode);

void inum_to_name(ino_t inode_to_find, char *name_buf, int buflen);

int main()
{
    ino_t this_inode = get_inode(".");
    char name_buf[BUFSIZ];
    get_path(name_buf, this_inode);
    printf("%s", name_buf);
    putchar('\n');
    return 0;
}

void get_path(char *result_buf, ino_t this_inode)
{
    char name_buf[BUFSIZ];
    ino_t parent_inode = get_inode("..");
    // not root
    if (parent_inode != this_inode) {
        chdir("..");
        inum_to_name(this_inode, name_buf, BUFSIZ);
        strcat(result_buf, "/");
        strcat(result_buf, name_buf);
        printf("%s\n", result_buf);
        fflush(stdout);
        ino_t next_inode = get_inode(".");
        get_path(result_buf, next_inode);
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
    printf("INTO inum_to_name inode_to_find: %lld\n", inode_to_find);
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
            printf("OUT inum_to_name d_name: %s, d_ino: %lld\n", direntp->d_name, direntp->d_ino);
            return;
        }
    }
    fprintf(stderr, "error looking for inum %lld\n", inode_to_find);
    _exit(1);
}

/**
 * returns inode number of the file
 * @param file_name
 * @return
 */
ino_t get_inode(char *file_name)
{
    struct stat info;

    if (stat(file_name, &info) == -1) {
        fprintf(stderr, "Cannot stat ");
        perror(file_name);
        _exit(1);
    }
    return info.st_ino;
}