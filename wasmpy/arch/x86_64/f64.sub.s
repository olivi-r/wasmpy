.globl _start
_start:
fldl (%rsp)
add $8, %rsp
fsubrl (%rsp)
fstpl (%rsp)
