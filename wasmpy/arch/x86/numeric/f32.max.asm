[BITS 32]
movss xmm0, [esp]
pop eax
movss xmm1, [esp]
maxss xmm0, xmm1
movss [esp], xmm0
