.globl _start
_start:
mov (%rsp), %eax
cbw
cwde
mov %eax, (%rsp)
