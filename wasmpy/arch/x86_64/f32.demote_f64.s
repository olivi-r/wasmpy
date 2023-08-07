.globl _start
_start:
cvtsd2ss (%rsp), %xmm0
addq $4, %rsp
movss %xmm0, (%rsp)
