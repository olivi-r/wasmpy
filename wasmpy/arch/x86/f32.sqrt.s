.globl _start
_start:
flds (%esp)
fsqrt
fstps (%esp)
