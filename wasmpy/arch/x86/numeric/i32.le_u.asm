[BITS 32]
pop eax
pop ecx
cmp ecx, eax
jbe true
push 0
jmp end
true:
push 1
end:
