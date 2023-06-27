.globl _start
_start:
.intel_syntax noprefix
pop ax
pop ax
shl eax, 16
pop ax
cmp eax, 0
je true
pushw 0
jmp end
true:
pushw 1
end:
pushw 0
pushw 2
