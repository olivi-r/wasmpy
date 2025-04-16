[BITS 64]
mov ecx, [rsp]
add rsp, 4
sar dword [rsp], cl
