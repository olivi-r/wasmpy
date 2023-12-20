.globl _start
_start:
mov (%rsp), %eax
mov %rax, (0xff00000000000000)
mov $0xff000000000000ff, %rax
leave
ret
