.globl _start
_start:
flds (%esp)
pop %eax
fdivrs (%esp)
fsts (%esp)
