.globl _start
_start:
cvtss2sd (%rsp), %xmm0
sub $4, %rsp
movsd %xmm0, (%rsp)
