[BITS 32]
pop eax
cmp eax, 0
pop eax
jne false
cmp eax, 0
jne false
push 1
jmp end
false:
push 0
end:
