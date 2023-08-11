.globl _start
_start:
fldl (%rsp)
fldl 8(%rsp)
add $12, %rsp
fcomip
fstps (%rsp)
jb true
movl $0, (%rsp)
jmp end
true:
movl $1, (%rsp)
end:
