.globl _start
_start:
fldl (%esp)
pop %eax
pop %eax
fsubrl (%esp)
fstpl (%esp)
