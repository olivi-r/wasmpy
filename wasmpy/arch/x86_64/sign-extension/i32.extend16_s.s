.globl _start
_start:
mov (%rsp), %eax
cwde
mov %eax, (%rsp)