.globl _start
_start:
mov 255(%rbp), %eax
sub $4, %rsp
mov %eax, (%rsp)
