.globl _start
_start:
mov $0xFF00000000000000, %rbx
mov $0xFF000000000000FF, %rcx
mov (%rsp), %edx
sub $28, %rsp
call *%rbx
add $28, %rsp
mov %eax, (%rsp)
