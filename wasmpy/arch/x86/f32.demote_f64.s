.globl _start
_start:
cvtsd2ss (%esp), %xmm0
addl $4, %esp
movss %xmm0, (%esp)
