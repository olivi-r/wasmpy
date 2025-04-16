[BITS 64]
movss xmm0, [rsp]
add rsp, 4
movss xmm1, [rsp]
maxss xmm0, xmm1
movss [rsp], xmm0
