flds (%esp)
pop %eax
flds (%esp)
fcomip
fstps (%esp)
ja true
movl $0, (%esp)
jmp end
true:
movl $1, (%esp)
end:
