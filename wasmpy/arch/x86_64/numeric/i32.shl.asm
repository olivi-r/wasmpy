[BITS 64]
mov ecx, [rsp]
add rsp, 4
shl dword [rsp], cl
