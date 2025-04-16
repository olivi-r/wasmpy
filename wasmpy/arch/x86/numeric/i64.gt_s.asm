[BITS 32]
pop eax
pop ecx
pop edx
pop ebx
cmp edx, eax
jg true
jl false
cmp ebx, ecx
ja true
false:
push 0
jmp end
true:
push 1
end:
