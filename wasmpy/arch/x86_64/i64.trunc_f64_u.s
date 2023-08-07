.globl _start
_start:
fldl (%rsp)
fabs
fisttpq (%rsp)
