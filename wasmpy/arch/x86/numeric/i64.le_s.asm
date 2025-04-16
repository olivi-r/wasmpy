[BITS 32]
pop eax
pop ecx
pop edx
pop ebx
cmp edx, eax
jl true
jg false
cmp ebx, ecx
jbe true
false:
push 0
jmp end
true:
push 1
end:
