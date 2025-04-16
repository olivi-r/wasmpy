[BITS 64]
mov ax, 6[rsp]
and ax, 0x7ff0
cmp ax, 0x7ff0
jne continue
mov rax, 0xff00000000000000
leave
ret
continue:
fld qword [rsp]
fabs
fisttp qword [rsp]
