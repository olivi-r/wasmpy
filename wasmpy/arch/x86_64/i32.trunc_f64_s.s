.globl _start
_start:
mov 6(%rsp), %ax
and $0x7ff0, %ax
cmp $0x7ff0, %ax
jne continue
mov $0xff00000000000000, %rax
leave
ret
continue:
fldl (%rsp)
add $4, %rsp
fisttpl (%rsp)
