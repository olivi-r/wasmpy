.globl _start
_start:
flds (%rsp)
fabs
fstps (%rsp)
