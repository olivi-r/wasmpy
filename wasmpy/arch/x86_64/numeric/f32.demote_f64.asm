[BITS 64]
cvtsd2ss xmm0, [rsp]
add rsp, 4
movss [rsp], xmm0
