.globl _start
_start:
fildq (%esp)
addl $4, %esp
fstps (%esp)
