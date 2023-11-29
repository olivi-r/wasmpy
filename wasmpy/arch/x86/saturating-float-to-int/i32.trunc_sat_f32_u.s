.globl _start
_start:
mov 2(%esp), %ax
and $0x7f80, %ax
cmp $0x7f80, %ax
jne continue
movl $0, (%esp)
jmp end
continue:
flds (%esp)
fabs
fisttpl (%esp)
end:
