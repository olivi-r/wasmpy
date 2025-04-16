[BITS 32]
pop eax
mov [dword 0xff000000], eax
pop eax
mov [dword 0xff0000ff], eax
mov eax, 0xff00ff00
leave
ret
