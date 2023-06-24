[bits 64]
pop ax
cmp ax, 4
je v64
pop ax
shl eax, 16
pop ax
push rbp
mov rcx, rsp
mov rsp, rbp
mov rbp, rcx
sub rsp, 0xff00ff
push ax
shr eax, 16
push ax
push strict word 2
mov rsp, rbp
pop rbp
jmp end
v64:
pop ax
shl eax, 16
pop ax
pop cx
shl ecx, 16
pop cx
push rbp
mov rdx, rsp
mov rsp, rbp
mov rbp, rdx
sub rsp, 0xff00ff
push cx
shr ecx, 16
push cx
push ax
shr eax, 16
push ax
push strict word 4
mov rsp, rbp
pop rbp
end:
