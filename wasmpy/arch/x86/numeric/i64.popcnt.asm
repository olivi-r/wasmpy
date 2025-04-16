[BITS 32]
pop eax
pop ecx
popcnt eax, eax
popcnt ecx, ecx
add eax, ecx
push 0
push eax
