[BITS 32]
cmp dword [esp], 0
jne continue
cmp dword 4[esp], 0
jne continue
mov eax, 0xFF000000
leave
ret
continue:
mov ebx, 0xFF0000FF
call ebx
pop ecx
pop ecx
push edx
push eax
