.globl _start
_start:
.intel_syntax noprefix
pop ax
cmp ax, 2
je x32
pop ax
pop ax
x32:
pop ax
pop ax
