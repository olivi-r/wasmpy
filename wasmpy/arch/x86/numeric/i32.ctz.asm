[BITS 32]
pop eax
cmp eax, 0
je zero
bsf eax, eax
push eax
jmp end
zero:
push 32
end:
