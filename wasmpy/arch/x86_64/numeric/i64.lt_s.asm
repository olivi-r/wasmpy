[BITS 64]
pop rax
pop rcx
cmp rcx, rax
sub rsp, 4
jl true
mov dword [rsp], 0
jmp end
true:
mov dword [rsp], 1
end:
