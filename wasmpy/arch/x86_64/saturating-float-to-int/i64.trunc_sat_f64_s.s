mov 6(%rsp), %ax
and $0x7ff0, %ax
cmp $0x7ff0, %ax
jne continue
movq $0, (%rsp)
jmp end
continue:
fldl (%rsp)
fisttpq (%rsp)
end:
