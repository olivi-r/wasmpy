.globl _start
_start:
mov (0xff00000000000000), %eax
sub $4, %rsp
mov %eax, (%rsp)
