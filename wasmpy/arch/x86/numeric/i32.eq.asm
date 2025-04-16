[BITS 32]
pop eax
pop ecx
cmp ecx, eax
je true
push 0
jmp end
true:
push 1
end:
