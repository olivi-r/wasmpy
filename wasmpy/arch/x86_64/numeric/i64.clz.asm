[BITS 64]
pop rax
cmp rax, 0
je zero
bsr rax, rax
mov rcx, 63
sub rcx, rax
push rcx
jmp end
zero:
push 64
end:
