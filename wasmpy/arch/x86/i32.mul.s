.globl _start
_start:
pop %eax
pop %ecx
imul %ecx, %eax
push %eax
