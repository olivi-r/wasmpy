.globl _start
_start:
flds (%esp)
push %eax
fabs
fisttpq (%esp)
