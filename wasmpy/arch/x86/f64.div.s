.globl _start
_start:
fldl (%esp)
pop %eax
pop %eax
fdivrl (%esp)
fstl (%esp)
