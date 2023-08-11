.globl _start
_start:
mov (%rsp), %ecx
add $4, %rsp
shrl %cl, (%rsp)
