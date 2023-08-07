.globl _start
_start:
subq $4, %rsp
movl $0x7fffffff, (%rsp)
