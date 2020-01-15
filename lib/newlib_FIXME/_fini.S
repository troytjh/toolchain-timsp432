        .global _fini

_fini:
@       l.movhi r3,hi(fini)
@       l.jal   _atexit
@       l.ori   r3,r3,lo(fini)          /* Delay slot */
