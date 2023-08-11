.globl _start
_start:
mov (%rsp), %eax
add $4, %rsp
or %eax, (%rsp)
