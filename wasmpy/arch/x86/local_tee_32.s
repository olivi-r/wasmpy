.globl _start
_start:
pop %eax
push %eax
mov %eax, 255(%ebp)
