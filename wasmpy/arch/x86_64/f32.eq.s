.globl _start
_start:
movl (%rsp), %eax
addq $4, %rsp
cmpl %eax, (%rsp)
je true
movl $0, (%rsp)
jmp end
true:
movl $1, (%rsp)
end:
