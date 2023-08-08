.globl _start
_start:
flds (%esp)
addl $4, %esp
fsubrs (%esp)
fstps (%esp)
