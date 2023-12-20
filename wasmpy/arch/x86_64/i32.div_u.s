.globl _start
_start:
mov 4(%rsp), %eax
cmpl $0, (%rsp)
jne continue
mov $0xff00000000000000, %rax
leave
ret
continue:
xor %edx, %edx
divl (%rsp)
add $4, %rsp
mov %eax, (%rsp)
