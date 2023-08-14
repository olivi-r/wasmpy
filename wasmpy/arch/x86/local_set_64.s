.globl _start
_start:
pop %eax
mov %eax, 254(%ebp)
pop %eax
mov %eax, 255(%ebp)
