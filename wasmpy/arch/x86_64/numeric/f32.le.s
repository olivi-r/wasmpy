flds (%rsp)
add $4, %rsp
flds (%rsp)
fcomip
fstps (%rsp)
jbe true
movl $0, (%rsp)
jmp end
true:
movl $1, (%rsp)
end:
