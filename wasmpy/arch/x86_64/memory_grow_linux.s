.globl _start
_start:
mov $0xFF00000000000000, %rbx
mov $0xFF000000000000FF, %rdi
mov (%rsp), %esi
call *%rbx
mov %eax, (%rsp)
