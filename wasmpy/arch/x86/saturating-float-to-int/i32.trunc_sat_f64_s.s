mov 6(%esp), %ax
and $0x7ff0, %ax
cmp $0x7ff0, %ax
add $4, %esp
jne continue
movl $0, (%esp)
jmp end
continue:
fldl -4(%esp)
fisttpl (%esp)
end:
