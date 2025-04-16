[BITS 64]
cvtss2sd xmm0, [rsp]
sub rsp, 4
movsd [rsp], xmm0
