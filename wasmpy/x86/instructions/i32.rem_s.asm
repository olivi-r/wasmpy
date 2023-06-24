[bits 32]
pop cx
pop cx
shl ecx, 16
pop cx
pop ax
pop ax
shl eax, 16
pop ax
cmp ecx, 0
je end
mov edx, 0
idiv ecx
mov eax, edx
push ax
shr eax, 16
push ax
push word 2
end:
