.globl _start
_start:
sub $4, %rsp
movl $0x7fffffff, (%rsp)
