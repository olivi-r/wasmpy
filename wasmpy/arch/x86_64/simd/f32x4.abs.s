.globl _start
_start:
andq $0x7fffffff7fffffff, (%rsp)
andq $0x7fffffff7fffffff, 8(%rsp)
