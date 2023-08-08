.globl _start
_start:
movss (%esp), %xmm0
addl $4, %esp
movss (%esp), %xmm1
minss %xmm1, %xmm0
movss %xmm0, (%esp)
