[BITS 64]
mov eax, [rsp]
add rsp, 4
xor [rsp], eax
