[BITS 32]
pop eax
pop ecx
pop edx
pop ebx
imul ecx, edx
imul ebx, eax
mul edx
add ecx, ebx
add edx, ecx
push edx
push eax
