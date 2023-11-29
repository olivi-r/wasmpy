.globl _start
_start:
mov 2(%esp), %ax
and $0x7f80, %ax
cmp $0x7f80, %ax
sub $4, %esp
jne continue
movl $0, (%esp)
movl $0, 4(%esp)
jmp end
continue:
flds 4(%esp)
fisttpq (%esp)
end:
