.globl _start
_start:
movsd (%esp), %xmm0
addl $8, %esp
movsd (%esp), %xmm1
minsd %xmm1, %xmm0
movsd %xmm0, (%esp)
