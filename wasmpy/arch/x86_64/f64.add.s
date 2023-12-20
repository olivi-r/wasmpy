.globl _start
_start:
fldl (%rsp)
add $8, %rsp
faddl (%rsp)
fstpl (%rsp)
