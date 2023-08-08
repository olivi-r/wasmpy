.globl _start
_start:
fildl (%esp)
subl $4, %esp
fstpl (%esp)
