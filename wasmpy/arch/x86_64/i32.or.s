.globl _start
_start:
movl (%rsp), %eax
addq $4, %rsp
orl %eax, (%rsp)
