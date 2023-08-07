.globl _start
_start:
cmpl $0, (%rsp)
je zero
addq $12, %rsp
jmp end
zero:
addq $4, %rsp
popq %rax
movq %rax, (%rsp)
end:
