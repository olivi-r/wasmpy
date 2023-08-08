.globl _start
_start:
movq $0xff00000000000000, %rax
leave
ret
