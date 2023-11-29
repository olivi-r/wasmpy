.globl _start
_start:
mov 2(%rsp), %ax
and $0x7f80, %ax
cmp $0x7f80, %ax
jne continue
movl $0, (%rsp)
jmp end
continue:
flds (%rsp)
fabs
fisttpl (%rsp)
end:
