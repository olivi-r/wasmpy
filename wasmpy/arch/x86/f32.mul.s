.globl _start
_start:
flds (%esp)
pop %eax
fmuls (%esp)
fsts (%esp)
