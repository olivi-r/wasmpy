[BITS 32]
mov ebx, 0xFF000000
push 0xFF0000FF
call ebx
pop ecx
pop ecx
push eax
