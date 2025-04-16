[BITS 32]
cmp dword [esp], 0
jne continue1
cmp dword 4[esp], 0
jne continue1
mov eax, 0xFF000000
leave
ret
continue1:
cmp dword [esp], 0xFFFFFFFF
jne continue2
cmp dword 4[esp], 0xFFFFFFFF
jne continue2
cmp dword 8[esp], 0
jne continue2
cmp dword 12[esp], 0x80000000
jne continue2
mov eax, 0xFF0000FF
leave
ret
continue2:
mov ebx, 0xFF00FF00
call ebx
pop ecx
pop ecx
push edx
push eax
