.globl _start
_start:
mov (%rsp), %ecx
add $4, %rsp
mov (%rsp), %eax
ror %cl, %eax
mov %eax, (%rsp)
