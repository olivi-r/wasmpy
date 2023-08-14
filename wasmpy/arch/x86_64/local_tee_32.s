.globl _start
_start:
mov (%rsp), %eax
mov %eax, 255(%rbp)
