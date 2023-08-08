.globl _start
_start:
fildl (%rsp)
subq $4, %rsp
fstpl (%rsp)
