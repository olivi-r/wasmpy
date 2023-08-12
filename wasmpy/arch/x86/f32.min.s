.globl _start
_start:
movss (%esp), %xmm0
pop %eax
movss (%esp), %xmm1
minss %xmm1, %xmm0
movss %xmm0, (%esp)
