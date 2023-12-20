.globl _start
_start:
cvtsd2ss (%esp), %xmm0
pop %eax
movss %xmm0, (%esp)
