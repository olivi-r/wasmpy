[BITS 32]
movsd xmm0, [esp]
pop eax
pop eax
movsd xmm1, [esp]
maxsd xmm0, xmm1
movsd [esp], xmm0
