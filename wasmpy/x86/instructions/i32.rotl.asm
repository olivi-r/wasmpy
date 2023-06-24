[bits 32]
pop cx
pop cx
shl ecx, 16
pop cx
pop ax
pop ax
shl eax, 16
pop ax
rol eax, cl
push ax
shr eax, 16
push ax
push word 2
