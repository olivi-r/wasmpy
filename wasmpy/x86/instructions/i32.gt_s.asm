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
jg true
push strict word 0
jmp end
true:
push strict word 1
end:
push strict word 0
push strict word 2
