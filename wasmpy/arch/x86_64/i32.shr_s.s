.globl _start
_start:
mov (%rsp), %ecx
add $4, %rsp
sarl %cl, (%rsp)
