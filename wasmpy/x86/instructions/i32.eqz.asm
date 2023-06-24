[bits 32]
pop ax
pop ax
shl eax, 16
pop ax
cmp eax, 0
je true
push strict word 0
jmp end
true:
push strict word 1
end:
push strict word 0
push strict word 2
