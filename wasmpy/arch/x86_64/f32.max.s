movss (%rsp), %xmm0
add $4, %rsp
movss (%rsp), %xmm1
maxss %xmm1, %xmm0
movss %xmm0, (%rsp)
