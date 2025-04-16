[BITS 32]
pop eax
mov [dword 0xff000000], eax
mov eax, 0xff0000ff
leave
ret
