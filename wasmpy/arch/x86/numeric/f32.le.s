flds (%esp)
pop %eax
flds (%esp)
fcomip
fstps (%esp)
jbe true
movl $0, (%esp)
jmp end
true:
movl $1, (%esp)
end:
