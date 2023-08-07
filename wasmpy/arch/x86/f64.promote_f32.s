.globl _start
_start:
cvtss2sd (%esp), %xmm0
subl $4, %esp
movsd %xmm0, (%esp)
