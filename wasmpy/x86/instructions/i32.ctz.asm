[bits 32]
pop ax
pop ax
shl eax, 16
pop ax
cmp eax, 0
je true
bsf eax, eax
push ax
jmp end
true:
push word 32
end:
push word 0
push word 2
