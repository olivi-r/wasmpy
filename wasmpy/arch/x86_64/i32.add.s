.globl _start
_start:
movl (%rsp), %eax
addq $4, %rsp
addl (%rsp), %eax
movl %eax, (%rsp)
