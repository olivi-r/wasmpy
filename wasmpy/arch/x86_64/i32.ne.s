.globl _start
_start:
movl (%rsp), %eax
addq $4, %rsp
cmpl %eax, (%rsp)
je false
movl $1, (%rsp)
jmp end
false:
movl $0, (%rsp)
end:
