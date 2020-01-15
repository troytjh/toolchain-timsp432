#include <errno.h>
#include <unistd.h>

#undef ERRNO
extern int  errno;

int _isatty(int file ) {
    if ((file == STDOUT_FILENO) || (file == STDERR_FILENO)) {
        return  1;
    }
    else {
        errno = EBADF;
        return  -1;
    }
}