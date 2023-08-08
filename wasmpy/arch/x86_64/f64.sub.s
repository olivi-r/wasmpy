.globl _start
_start:
fldl (%rsp)
addq $8, %rsp
fsubrl (%rsp)
fstpl (%rsp)
