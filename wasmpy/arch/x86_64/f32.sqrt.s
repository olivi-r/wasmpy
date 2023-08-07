.globl _start
_start:
flds (%rsp)
fsqrt
fstps (%rsp)
