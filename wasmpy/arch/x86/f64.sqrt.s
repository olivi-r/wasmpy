.globl _start
_start:
fldl (%esp)
fsqrt
fstpl (%esp)
