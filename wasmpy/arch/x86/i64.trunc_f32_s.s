.globl _start
_start:
flds (%esp)
subl $4, %esp
fisttpq (%esp)
