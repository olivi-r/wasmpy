.globl _start
_start:
mov $0xFF00000000000000, %rax
mov (%rax), %eax
sub $4, %rsp
mov %eax, (%rsp)
