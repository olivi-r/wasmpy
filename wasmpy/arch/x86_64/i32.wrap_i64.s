.globl _start
_start:
pop %rax
sub $4, %rsp
mov %eax, (%rsp)
