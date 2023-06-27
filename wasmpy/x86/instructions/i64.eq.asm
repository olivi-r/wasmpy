.globl _start
_start:
.intel_syntax noprefix
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
pushw 1
jmp end
false:
pushw 0
end:
pushw 0
pushw 2
