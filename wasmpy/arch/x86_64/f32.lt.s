.globl _start
_start:
flds (%rsp)
add $4, %rsp
flds (%rsp)
fcomip
fstps (%rsp)
jb true
movl $0, (%rsp)
jmp end
true:
movl $1, (%rsp)
end:
