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
cmp eax, ecx
jne false
cmp eax, 0
jne false
pushw 1
jmp end
false:
pushw 0
end:
pushw 0
pushw 2
