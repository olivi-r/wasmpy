.globl _start
_start:
flds (%rsp)
subq $4, %rsp
fabs
fisttpq (%rsp)
