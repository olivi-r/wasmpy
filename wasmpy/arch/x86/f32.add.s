.globl _start
_start:
flds (%esp)
addl $4, %esp
fadds (%esp)
fstps (%esp)
