.globl _start
_start:
cmpl $0, (%rsp)
je zero
addq $8, %rsp
jmp end
zero:
movl 4(%rsp), %eax
addq $8, %rsp
movl %eax, (%rsp)
end:
