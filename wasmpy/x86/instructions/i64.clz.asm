[bits 32]
pop ax
pop ax
shl eax, 16
pop ax
pop cx
shl ecx, 16
pop cx
cmp eax, 0
je highzero
bsr eax, eax
mov cx, 31
sub cx, ax
push cx
jmp end
highzero:
cmp ecx, 0
je lowzero
bsr ecx, ecx
mov ax, 63
sub ax, cx
push ax
jmp end
lowzero:
push strict word 64
end:
push strict word 0
push strict word 0
push strict word 0
push strict word 4
