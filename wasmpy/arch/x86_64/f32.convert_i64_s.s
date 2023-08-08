.globl _start
_start:
fildq (%rsp)
addq $4, %rsp
fstps (%rsp)
