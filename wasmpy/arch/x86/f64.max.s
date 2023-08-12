.globl _start
_start:
movsd (%esp), %xmm0
pop %eax
pop %eax
movsd (%esp), %xmm1
maxsd %xmm1, %xmm0
movsd %xmm0, (%esp)
