.globl _start
_start:
fldl (%esp)
pop %eax
pop %eax
fmull (%esp)
fstl (%esp)
