#define UNHANDLED_EXCEPTION(str)                                         \
        l.addi  r1,r1,-20               /* Standard prologue */         ;\
        l.sw    16(r1),r2                                               ;\
        l.addi  r2,r1,20                                                ;\
        l.sw    12(r1),r9                                               ;\
                                                                        ;\
        l.movhi r3,hi(.Lfmt)            /* printf format string */      ;\
        l.ori   r3,r3,lo(.Lfmt)                                         ;\
        l.sw    0(r1),r3                                                ;\
        l.movhi r4,hi(str)              /* Name of exception */         ;\
        l.ori   r4,r4,lo(str)                                           ;\
        l.sw    4(r1),r4                                                ;\
        l.mfspr r5,r0,SPR_EPCR_BASE     /* Source of the interrupt */   ;\
        l.jal   _printf                                                 ;\
        l.sw    8(r1),r5                                                ;\
                                                                        ;\
        l.ori   r3,r0,0xffff            /* Failure RC */                ;\
        l.jal   _exit                                                   ;\
        l.nop                                                           ;\
                                                                        ;\
        l.rfe                           /* Never executed we hope */ 
