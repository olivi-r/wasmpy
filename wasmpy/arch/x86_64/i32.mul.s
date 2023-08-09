.globl _start
_start:
mov (%rsp), %eax
add $4, %rsp
imul (%rsp), %eax
mov %eax, (%rsp)
