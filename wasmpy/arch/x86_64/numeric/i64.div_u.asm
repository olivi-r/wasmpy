[BITS 64]
pop rcx
pop rax
cmp rcx, 0
jne continue
mov rax, 0xff00000000000000
leave
ret
continue:
xor rdx, rdx
div rcx
push rax
