[BITS 64]
mov eax, [rsp]
add rsp, 4
imul dword [rsp]
mov [rsp], eax
