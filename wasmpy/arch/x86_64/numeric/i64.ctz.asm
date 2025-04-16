[BITS 64]
pop rax
cmp rax, 0
je zero
bsf rax, rax
push rax
jmp end
zero:
push 64
end:
