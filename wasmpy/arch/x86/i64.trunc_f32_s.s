.globl _start
_start:
flds (%esp)
push %eax
fisttpq (%esp)
