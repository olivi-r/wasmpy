.globl _start
_start:
mov 127(%rbp), %eax
sub $4, %rsp
mov %eax, (%rsp)
