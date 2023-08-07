.globl _start
_start:
fldl (%rsp)
fabs
fstpl (%rsp)
