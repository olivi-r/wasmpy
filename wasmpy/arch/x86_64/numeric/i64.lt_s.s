pop %rax
pop %rcx
cmp %rax, %rcx
sub $4, %rsp
jl true
movl $0, (%rsp)
jmp end
true:
movl $1, (%rsp)
end:
