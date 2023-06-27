.globl _start
_start:
.intel_syntax noprefix
pop ax
pop ax
shl eax, 16
pop ax
pop cx
pop cx
shl ecx, 16
pop cx
cmp eax, ecx
je false
pushw 1
jmp end
false:
pushw 0
end:
pushw 0
pushw 2
