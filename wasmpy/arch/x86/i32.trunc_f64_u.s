.globl _start
_start:
fldl (%esp)
addl $4, %esp
fabs
fisttpl (%esp)
