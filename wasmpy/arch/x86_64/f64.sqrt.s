.globl _start
_start:
fldl (%rsp)
fsqrt
fstpl (%rsp)
