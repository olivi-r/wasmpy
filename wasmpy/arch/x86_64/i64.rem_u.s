.globl _start
_start:
pop %rcx
pop %rax
cmp $0, %rcx
jne continue
mov $0xff00000000000000, %rax
leave
ret
continue:
xor %rdx, %rdx
div %rcx
push %rdx
