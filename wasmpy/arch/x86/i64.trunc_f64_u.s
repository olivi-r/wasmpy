.globl _start
_start:
fldl (%esp)
fabs
fisttpq (%esp)
