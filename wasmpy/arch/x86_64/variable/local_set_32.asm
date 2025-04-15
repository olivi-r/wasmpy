[BITS 64]
mov eax, [rsp]
add rsp, 4
mov 255[rbp], eax
