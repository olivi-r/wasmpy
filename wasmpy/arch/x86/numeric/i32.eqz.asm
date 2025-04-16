[BITS 32]
pop eax
cmp eax, 0
je true
push 0
jmp end
true:
push 1
end:
