.globl _start
_start:
flds (%esp)
add $4, %esp
fdivrs (%esp)
fsts (%esp)
