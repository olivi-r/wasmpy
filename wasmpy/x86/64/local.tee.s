.globl _start
_start:
.intel_syntax noprefix
pop ax
cmp ax, 4
je v64
pop ax
shl eax, 16
pop ax
mov ecx, eax
push cx
shr ecx, 16
push cx
pushw 2
push rbp
mov rcx, rsp
mov rsp, rbp
mov rbp, rcx
sub rsp, 0xff00ff
push ax
shr eax, 16
push ax
pushw 2
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
mov edx, eax
mov ebx, ecx
push bx
shr ebx, 16
push bx
push dx
shr edx, 16
push dx
pushw 4
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
pushw 4
mov rsp, rbp
pop rbp
end:
