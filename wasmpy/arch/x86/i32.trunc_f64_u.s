.globl _start
_start:
fldl (%esp)
pop %eax
fabs
fisttpl (%esp)
