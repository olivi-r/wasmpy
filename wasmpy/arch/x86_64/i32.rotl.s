.globl _start
_start:
mov (%rsp), %ecx
add $4, %rsp
mov (%rsp), %eax
rol %cl, %eax
mov %eax, (%rsp)
