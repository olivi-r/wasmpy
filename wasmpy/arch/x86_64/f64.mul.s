.globl _start
_start:
fldl (%rsp)
addq $8, %rsp
fmull (%rsp)
fstl (%rsp)
