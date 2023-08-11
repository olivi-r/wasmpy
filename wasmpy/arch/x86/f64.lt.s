.globl _start
_start:
fldl (%esp)
fldl 8(%esp)
add $12, %esp
fcomip
fstps (%esp)
jb true
movl $0, (%esp)
jmp end
true:
movl $1, (%esp)
end:
