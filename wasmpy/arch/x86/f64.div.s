.globl _start
_start:
fldl (%esp)
add $8, %esp
fdivrl (%esp)
fstl (%esp)
