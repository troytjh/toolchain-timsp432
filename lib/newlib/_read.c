#include "UART/UART_IO.h"
#include <errno.h>
#include <unistd.h>

#undef errno
extern int  errno;

int _read(int file, char *ptr, int len) {
    OutputInit();
    if(STDIN_FILENO==file) {
        int i;
        for (i = 0; i < len; ++i) {
            ptr[i] = ReadInput();

            SendOutput(ptr[i]);
            /* Return partial buffer if we get EOL */
            if ('\r' == ptr[i]) {
                SendOutput('\n');
                SendOutput('\r');
                return  i;
            }
        }
        return i;
    }
    else {
        errno = EBADF;
        return -1;
    }
}