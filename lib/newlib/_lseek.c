#include <errno.h>
#include <unistd.h>

#undef errno
extern int errno;

int _lseek(int file, int offset, int whence) {
    if ((STDOUT_FILENO == file) || (STDERR_FILENO == file)) {
        return  0;
    }
    else {
      errno = EBADF;
      return  (long) -1;
    }
}
