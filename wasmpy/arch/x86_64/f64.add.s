.globl _start
_start:
fldl (%rsp)
addq $8, %rsp
faddl (%rsp)
fstpl (%rsp)
