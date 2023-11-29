.globl _start
_start:
mov 2(%rsp), %ax
and $0x7f80, %ax
cmp $0x7f80, %ax
sub $4, %rsp
jne continue
movq $0, (%rsp)
jmp end
continue:
flds 4(%rsp)
fisttpq (%rsp)
end:
