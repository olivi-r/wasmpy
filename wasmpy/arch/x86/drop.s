.globl _start
_start:
pop %ax
cmp $2, %ax
je x32
pop %ax
pop %ax
x32:
pop %ax
pop %ax
