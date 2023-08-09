.globl _start
_start:
fldl (%esp)
addl $8, %esp
fmull (%esp)
fstl (%esp)
