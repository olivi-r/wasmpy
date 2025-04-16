[BITS 32]
cvtss2sd xmm0, [esp]
push eax
movsd [esp], xmm0
