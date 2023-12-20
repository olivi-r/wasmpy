.globl _start
_start:
cvtss2sd (%esp), %xmm0
push %eax
movsd %xmm0, (%esp)
