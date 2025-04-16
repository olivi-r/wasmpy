[BITS 32]
pop eax
pop ecx
pop esi
pop edi
cmp esi, eax
jne true
cmp edi, ecx
jne true
push 0
jmp end
true:
push 1
end:
