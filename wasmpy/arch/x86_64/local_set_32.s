.globl _start
_start:
mov (%rsp), %eax
add $4, %rsp
mov %eax, 255(%rbp)