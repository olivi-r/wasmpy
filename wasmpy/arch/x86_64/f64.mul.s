.globl _start
_start:
fldl (%rsp)
add $8, %rsp
fmull (%rsp)
fstl (%rsp)
