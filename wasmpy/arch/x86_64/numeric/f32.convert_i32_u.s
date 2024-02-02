mov (%rsp), %eax
cvtsi2ss %rax, %xmm0
movss %xmm0, (%rsp)
