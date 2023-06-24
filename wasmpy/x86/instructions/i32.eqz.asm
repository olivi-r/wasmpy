[bits 32]
pop ax
pop ax
shl eax, 16
pop ax
cmp eax, 0
je true
push word 0
jmp end
true:
push word 1
end:
push word 0
push word 2
