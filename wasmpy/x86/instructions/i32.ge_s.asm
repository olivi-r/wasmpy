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
cmp ecx, eax
jge true
pushw 0
jmp end
true:
pushw 1
end:
pushw 0
pushw 2
