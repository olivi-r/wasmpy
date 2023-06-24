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
jne false
cmp ecx, ebx
jne false
push strict word 1
jmp end
false:
push strict word 0
end:
push strict word 0
push strict word 2
