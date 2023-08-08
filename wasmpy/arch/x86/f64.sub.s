.globl _start
_start:
fldl (%esp)
addl $8, %esp
fsubrl (%esp)
fstpl (%esp)
