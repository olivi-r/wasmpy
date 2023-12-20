.globl _start
_start:
pop %eax
mov %eax, 254(%ebp)
pop %ecx
mov %ecx, 255(%ebp)
push %ecx
push %eax
