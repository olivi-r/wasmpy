.globl _start
_start:
cmpl $0, 2(%rsp)
je zero
addq $12, %rsp
jmp end
zero:
movl 8(%rsp), %eax
movl %eax, 14(%rsp)
addq $12, %rsp
end:
