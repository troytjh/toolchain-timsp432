#ifndef UART_FUNC
#define UART_FUNC

#include "msp.h"

void OutputInit(void);

unsigned char ReadInput(void);

int SendOutput(unsigned char c);

#endif