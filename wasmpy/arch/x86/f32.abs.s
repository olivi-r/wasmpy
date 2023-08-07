.globl _start
_start:
flds (%esp)
fabs
fstps (%esp)
