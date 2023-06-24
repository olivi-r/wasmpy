[bits 64]
push rbp
mov rax, rsp
mov rsp, rbp
mov rbp, rax
sub rsp, 0xff00ff
pop ax
cmp ax, 4
je v64
pop ax
pop ax
pop ax
shl eax, 16
pop ax
mov rsp, rbp
pop rbp
push ax
shr eax, 16
push ax
push strict word 2
jmp end
v64:
pop ax
shl eax, 16
pop ax
pop cx
shl ecx, 16
pop cx
mov rsp, rbp
pop rbp
push cx
shr ecx, 16
push cx
push ax
shr eax, 16
push ax
push strict word 4
end:
