[BITS 64]
mov eax, [rsp]
add rsp, 4
mov [qword 0xff00000000000000], eax
