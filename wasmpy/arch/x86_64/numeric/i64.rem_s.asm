[BITS 64]
pop rcx
pop rax
cmp rcx, 0
je zero
mov rdx, 0x8000000000000000
cmp rax, rdx
jne cont
cmp rcx, 0xffffffffffffffff
jne cont
push 0
jmp end
zero:
mov rax, 0xff00000000000000
leave
ret
cont:
cqo
idiv rcx
push rdx
end:
