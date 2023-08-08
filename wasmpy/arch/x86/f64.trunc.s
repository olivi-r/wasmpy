.globl _start
_start:
fldl (%esp)
fisttpq (%esp)
fildl (%esp)
fstpl (%esp)
