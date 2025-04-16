[BITS 64]
mov ax, 2[rsp]
and ax, 0x7f80
cmp ax, 0x7f80
jne continue
mov rax, 0xff00000000000000
leave
ret
continue:
fld dword [rsp]
fisttp dword [rsp]
