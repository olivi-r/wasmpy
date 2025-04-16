[BITS 32]
pop eax
pop ecx
cmp eax, ecx
je false
push 1
jmp end
false:
push 0
end:
