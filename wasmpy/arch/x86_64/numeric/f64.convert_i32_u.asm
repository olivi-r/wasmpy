[BITS 64]
mov eax, [rsp]
cvtsi2sd xmm0, rax
sub rsp, 4
movsd [rsp], xmm0
