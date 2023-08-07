.globl _start
_start:
cvtss2sd (%rsp), %xmm0
subq $4, %rsp
movsd %xmm0, (%rsp)
