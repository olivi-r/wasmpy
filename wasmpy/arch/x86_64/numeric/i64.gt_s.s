pop %rax
pop %rcx
cmp %rax, %rcx
sub $4, %rsp
jg true
movl $0, (%rsp)
jmp end
true:
movl $1, (%rsp)
end:
