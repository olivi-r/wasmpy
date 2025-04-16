[BITS 64]
pop rax
sub rsp, 4
cmp rax, 0
je true
mov dword [rsp], 0
jmp end
true:
mov dword [rsp], 1
end:
