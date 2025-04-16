[BITS 32]
pop ecx
pop eax
cmp eax, 0
je highzero
bsf eax, eax
add eax, 32
push 0
push eax
jmp end
highzero:
cmp ecx, 0
je lowzero
bsf ecx, ecx
push 0
push ecx
jmp end
lowzero:
push 0
push 64
end:
