[bits 32]
pop ax
pop ax
shl eax, 16
pop ax
pop cx
shl ecx, 16
pop cx
pop dx
pop dx
shl edx, 16
pop dx
pop bx
shl ebx, 16
pop bx
cmp eax, edx
jne true
cmp ecx, ebx
jne true
push word 0
jmp end
true:
push word 1
end:
push word 0
push word 2
