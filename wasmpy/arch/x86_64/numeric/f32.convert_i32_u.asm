[BITS 64]
mov eax, [rsp]
cvtsi2ss xmm0, rax
movss [rsp], xmm0
