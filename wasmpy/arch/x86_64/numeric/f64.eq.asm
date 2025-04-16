[BITS 64]
pop rax
pop rcx
sub rsp, 4
cmp rcx, rax
je true
mov dword [rsp], 0
jmp end
true:
mov dword [rsp], 1
end:
