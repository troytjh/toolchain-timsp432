        .global _longjmp
_longjmp:
        /* Sort out the return value */
@        l.sfne  r4,r0
@        l.bf    1f
@        l.nop

@        l.j     2f
@        l.addi  r11,r0,1                /* 1 as result */

1:@      l.addi  r11,r4,0                /* val as result */

        /* Restore all the other registers, leaving r3 to last. */
2:@      l.lwz   r31,120(r3)
@        l.lwz   r30,116(r3)
@        l.lwz   r29,112(r3)
@        l.lwz   r28,108(r3)
@        l.lwz   r27,104(r3)
@        l.lwz   r26,100(r3)
@        l.lwz   r25,96(r3)
@        l.lwz   r24,92(r3)
@        l.lwz   r23,88(r3)
@        l.lwz   r22,84(r3)
@        l.lwz   r21,80(r3)
@        l.lwz   r20,76(r3)
@        l.lwz   r19,72(r3)
@        l.lwz   r18,68(r3)
@        l.lwz   r17,64(r3)
@        l.lwz   r16,60(r3)
@        l.lwz   r15,56(r3)
@        l.lwz   r14,52(r3)
@        l.lwz   r13,48(r3)
@        l.lwz   r12,44(r3)
@        l.lwz   r10,40(r3)              /* Omit r11 */
@        l.lwz   r9,36(r3)
@        l.lwz   r8,32(r3)
@        l.lwz   r7,28(r3)
@        l.lwz   r6,24(r3)
@        l.lwz   r5,20(r3)
@        l.lwz   r4,16(r3)
@        l.lwz   r2,8(r3)                /* Skip r3 */
@        l.lwz   r1,4(r3)                /* Slot 0 saved for flag in future */
@        l.lwz   r3,12(r3)               /* Now safe */
        
        /* Result is already in r11. Having restored r9, it will appear as
           though we have returned from the earlier call to _setjmp. The
           non-zero result gives it away though. */
@        l.jr    r9
@        l.nop
