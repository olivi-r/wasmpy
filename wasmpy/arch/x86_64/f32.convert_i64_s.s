.globl _start
_start:
fildq (%rsp)
add $4, %rsp
fstps (%rsp)
