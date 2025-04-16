[BITS 64]
mov ecx, [rsp]
add rsp, 4
rol dword [rsp], cl
