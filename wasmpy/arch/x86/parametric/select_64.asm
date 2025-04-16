[BITS 32]
pop eax
cmp eax, 0
pop eax
pop ecx
je zero
jmp end
zero:
pop edx
pop edx
push ecx
push eax
end:
