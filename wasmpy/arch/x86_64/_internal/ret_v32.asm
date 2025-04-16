[BITS 64]
mov eax, [rsp]
mov [qword 0xff00000000000000], rax
mov rax, 0xff000000000000ff
leave
ret
