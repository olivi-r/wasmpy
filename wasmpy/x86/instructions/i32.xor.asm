[bits 32]
pop ax
pop ax
shl eax, 16
pop ax
pop cx
pop cx
shl ecx, 16
pop cx
xor eax, ecx
push ax
shr eax, 16
push ax
push word 2
