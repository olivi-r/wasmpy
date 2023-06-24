[bits 32]
pop ax
pop ax
shl eax, 16
pop ax
pop cx
pop cx
shl ecx, 16
pop cx
cmp eax, ecx
je false
push word 1
jmp end
false:
push word 0
end:
push word 0
push word 2
