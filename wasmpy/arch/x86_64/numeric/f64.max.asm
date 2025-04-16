[BITS 64]
movsd xmm0, [rsp]
add rsp, 8
movsd xmm1, [rsp]
maxsd xmm0, xmm1
movsd [rsp], xmm0
