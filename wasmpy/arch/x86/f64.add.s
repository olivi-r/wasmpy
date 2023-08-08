.globl _start
_start:
fldl (%esp)
addl $8, %esp
faddl (%esp)
fstpl (%esp)
