.globl _start
_start:
mov 2(%rsp), %ax
and $0x7f80, %ax
cmp $0x7f80, %ax
jne continue
mov $0xff00000000000000, %rax
leave
ret
continue:
flds (%rsp)
sub $4, %rsp
fabs
fisttpq (%rsp)
