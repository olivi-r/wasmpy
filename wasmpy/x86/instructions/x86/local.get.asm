.globl _start
_start:
.intel_syntax noprefix
push ebp
mov eax, esp
mov esp, ebp
mov ebp, eax
sub esp, 0xff00ff
pop ax
cmp ax, 4
je v64
pop ax
pop ax
pop ax
shl eax, 16
pop ax
mov esp, ebp
pop ebp
push ax
shr eax, 16
push ax
pushw 2
jmp end
v64:
pop ax
shl eax, 16
pop ax
pop cx
shl ecx, 16
pop cx
mov esp, ebp
pop ebp
push cx
shr ecx, 16
push cx
push ax
shr eax, 16
push ax
pushw 4
end:
