.globl _start
_start:
flds (%rsp)
fabs
fisttpl (%rsp)
