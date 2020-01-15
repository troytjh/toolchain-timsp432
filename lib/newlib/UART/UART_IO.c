#include "UART_IO.h"

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