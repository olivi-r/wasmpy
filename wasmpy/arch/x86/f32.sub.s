.globl _start
_start:
flds (%esp)
pop %eax
fsubrs (%esp)
fstps (%esp)
