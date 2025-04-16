[BITS 64]
pop rax
pop rcx
sub rsp, 4
cmp rax, rcx
je false
mov dword [rsp], 1
jmp end
false:
mov dword [rsp], 0
end:
