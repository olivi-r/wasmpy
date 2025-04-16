[BITS 64]
mov ecx, [rsp]
add rsp, 4
ror dword [rsp], cl
