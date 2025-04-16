[BITS 32]
pop ecx
pop eax
cmp ecx, 0
je zero
cmp eax, 0x80000000
jne cont
cmp ecx, 0xffffffff
jne cont
push 0
jmp end
zero:
mov eax, 0xff000000
leave
ret
cont:
cdq
idiv ecx
push edx
end:
