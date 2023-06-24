[bits 32]
mov cx, 32
pop dx
pop dx
pop ax
idiv cx
pop ax
pop ax
shl eax, 16
pop ax
mov cl, dl
sar eax, cl
push ax
shr eax, 16
push ax
push strict word 2
