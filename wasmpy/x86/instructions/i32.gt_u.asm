[bits 32]
pop ax
pop ax
shl eax, 16
pop ax
pop cx
pop cx
shl ecx, 16
pop cx
cmp ecx, eax
ja true
push word 0
jmp end
true:
push word 1
end:
push word 0
push word 2
