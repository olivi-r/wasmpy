[BITS 32]
pop eax
pop ecx
pop esi
pop edi
cmp esi, eax
jne false
cmp edi, ecx
jne false
push 1
jmp end
false:
push 0
end:
