.globl _start
_start:
mov $0xFF000000, %ebx
pushl $0xFF0000FF
call *%ebx
pop %ecx
pop %ecx
push %eax
