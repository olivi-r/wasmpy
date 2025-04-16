[BITS 64]
mov eax, [rsp]
add rsp, 4
add [rsp], eax
