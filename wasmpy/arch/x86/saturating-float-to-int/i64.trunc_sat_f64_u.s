mov 6(%esp), %ax
and $0x7ff0, %ax
cmp $0x7ff0, %ax
jne continue
movl $0, (%esp)
movl $0, 4(%esp)
jmp end
continue:
fldl (%esp)
fabs
fisttpq (%esp)
end: