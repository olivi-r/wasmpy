[bits 32]
pop ax
cmp ax, 4
je v64
pop ax
shl eax, 16
pop ax
push ebp
mov ecx, esp
mov esp, ebp
mov ebp, ecx
sub esp, 0xff00ff
push ax
shr eax, 16
push ax
push strict word 2
mov esp, ebp
pop ebp
jmp end
v64:
pop ax
shl eax, 16
pop ax
pop cx
shl ecx, 16
pop cx
push ebp
mov edx, esp
mov esp, ebp
mov ebp, edx
sub esp, 0xff00ff
push cx
shr ecx, 16
push cx
push ax
shr eax, 16
push ax
push strict word 4
mov esp, ebp
pop ebp
end:
