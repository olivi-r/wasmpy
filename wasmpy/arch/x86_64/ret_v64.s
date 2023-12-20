.globl _start
_start:
pop %rax
mov %rax, (0xff00000000000000)
mov $0xff000000000000ff, %rax
leave
ret
