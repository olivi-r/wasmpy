.globl _start
_start:
mov (%rsp), %eax
cvtsi2ss %rax, %xmm0
movss %xmm0, (%rsp)
