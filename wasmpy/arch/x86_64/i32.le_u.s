mov (%rsp), %eax
add $4, %rsp
cmp %eax, (%rsp)
jbe true
movl $0, (%rsp)
jmp end
true:
movl $1, (%rsp)
end:
