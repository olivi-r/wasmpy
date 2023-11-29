.globl _start
_start:
mov 6(%rsp), %ax
and $0x7ff0, %ax
cmp $0x7ff0, %ax
add $4, %rsp
jne continue
movl $0, (%rsp)
jmp end
continue:
fldl -4(%rsp)
fisttpl (%rsp)
end:
