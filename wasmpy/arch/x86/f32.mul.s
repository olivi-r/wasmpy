.globl _start
_start:
flds (%esp)
addl $4, %esp
fmuls (%esp)
fsts (%esp)
