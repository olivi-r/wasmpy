[BITS 32]
pop eax
pop ecx
pop edx
pop ebx
cmp edx, eax
jb true
ja false
cmp ebx, ecx
jb true
false:
push 0
jmp end
true:
push 1
end:
