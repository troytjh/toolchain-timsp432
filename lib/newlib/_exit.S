    .global _exit
_exit:
@   t1(r3)  .int    =rc
@   l.nop   K(NOP_EXIT),r(t1)