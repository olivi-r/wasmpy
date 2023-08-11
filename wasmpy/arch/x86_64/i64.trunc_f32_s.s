.globl _start
_start:
flds (%rsp)
sub $4, %rsp
fisttpq (%rsp)
