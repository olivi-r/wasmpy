[BITS 32]
cvtsd2ss xmm0, [esp]
pop eax
movss [esp], xmm0
