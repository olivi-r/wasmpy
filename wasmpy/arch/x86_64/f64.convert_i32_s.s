.globl _start
_start:
fildl (%rsp)
sub $4, %rsp
fstpl (%rsp)
