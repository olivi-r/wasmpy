.globl _start
_start:
fldl (%esp)
addl $4, %esp
fisttpl (%esp)
