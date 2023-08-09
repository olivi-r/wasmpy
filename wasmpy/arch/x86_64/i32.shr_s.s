.globl _start
_start:
mov (%rsp), %ecx
add $4, %rsp
mov (%rsp), %eax
sar %cl, %eax
mov %eax, (%rsp)
