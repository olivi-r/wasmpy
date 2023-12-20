.globl _start
_start:
movl (%rsp), %eax
cvtsi2sd %rax, %xmm0
sub $4, %rsp
movsd %xmm0, (%rsp)
