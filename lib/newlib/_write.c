#include "UART/UART_IO.h"

int _write(int file, char *ptr, int len) {
    /* Implement your write code here, this is used by puts and printf for example */
    OutputInit();
    for(int i=0;i<len;++i) {
        SendOutput(*ptr++);
    }
    return len;
}