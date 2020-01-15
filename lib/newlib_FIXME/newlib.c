/* Do not modify any code. This is to redirect
printf to the console window with Launchpad*/

#include "msp.h"
#include <stdio.h>
#include <string.h>

#include <newlib.h>
#include <setjmp.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>

void OutputInit(void) {
    EUSCI_A0->CTLW0 |= 1;       
    EUSCI_A0->MCTLW = 0;         
    EUSCI_A0->CTLW0 = 0x0081;   
    EUSCI_A0->BRW = 26;         
    P1->SEL0 |= 0x0C;           
    P1->SEL1 &= ~0x0C;
    EUSCI_A0->CTLW0 &= ~1;      
}

/* Receive from PC */
unsigned char ReadInput(void) {
    char c;

    while(!(EUSCI_A0->IFG & 0x01));
    c = EUSCI_A0->RXBUF;
    return c;
}

/* Send to PC */
int SendOutput(unsigned char c) {
    while(!(EUSCI_A0->IFG&0x02));
    EUSCI_A0->TXBUF = c;
    return c;
}

/* The code below is the interface to the C standard I/O library.*/
int _write(int file, char *ptr, int len)
{
    /* Implement your write code here, this is used by puts and printf for example */
    OutputInit();
    for(int i=0;i<len;++i) {
        SendOutput(*ptr++);
    }
    return len;
}

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


#define STACK_BUFFER  65536

void *_sbrk(int nbytes) {
  // Symbol defined by linker map 
  extern int  end;              // start of free memory (as symbol) 

  // Value set by crt0.S 
  extern void *_stack;           // end of free memory 

  // The statically held previous end of the heap, with its initialization. 
  static void *heap_ptr = (void *)&end;         // Previous end 

  if ((_stack - (heap_ptr + nbytes)) > STACK_BUFFER )
    {
      void *base  = heap_ptr;
      heap_ptr   += nbytes;
                
      return  base;
    }
  else
    {
      errno = ENOMEM;
      return  (void *) -1;
    }
}


#undef errno
extern int errno;

int _close(int file) {

    errno=EBADF;

    return -1;
}

int _fstat(int file, struct stat *st) {
    st->st_mode = S_IFCHR;
    return 0;
}

int _isatty(int file ) {
    if ((file == STDOUT_FILENO) || (file == STDERR_FILENO)) {
        return  1;
    }
    else {
        errno = EBADF;
        return  -1;
    }
}

int _lseek(int file, int offset, int whence) {
    if ((STDOUT_FILENO == file) || (STDERR_FILENO == file)) {
        return  0;
    }
    else {
      errno = EBADF;
      return  (long) -1;
    }
}