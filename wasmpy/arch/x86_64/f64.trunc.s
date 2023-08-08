.globl _start
_start:
fldl (%rsp)
fisttpq (%rsp)
fildl (%rsp)
fstpl (%rsp)
