[BITS 64]
mov ecx, [rsp]
add rsp, 4
shr dword [rsp], cl
