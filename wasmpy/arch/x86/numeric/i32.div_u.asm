[BITS 32]
pop ecx
pop eax
cmp ecx, 0
jne continue
mov eax, 0xff000000
leave
ret
continue:
xor edx, edx
div ecx
push eax
