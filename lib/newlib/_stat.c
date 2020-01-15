#include <errno.h>
#include <sys/stat.h>

#undef errno
extern int  errno;

int
_stat (char *file, struct stat *st) {
    errno = EACCES;
    return  -1;
}       /* _stat () */