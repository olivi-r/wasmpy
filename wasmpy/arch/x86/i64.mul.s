.globl _start
_start:
pop %eax
pop %ecx
pop %edx
pop %ebx
imul %edx, %ecx
imul %eax, %ebx
mul %edx
add %ebx, %ecx
add %ecx, %edx
push %edx
push %eax
