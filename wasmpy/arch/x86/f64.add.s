.globl _start
_start:
fldl (%esp)
pop %eax
pop %eax
faddl (%esp)
fstpl (%esp)
