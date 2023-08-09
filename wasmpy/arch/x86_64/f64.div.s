.globl _start
_start:
fldl (%rsp)
add $8, %rsp
fdivrl (%rsp)
fstl (%rsp)
