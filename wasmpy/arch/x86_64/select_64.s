.globl _start
_start:
cmpl $0, 2(%rsp)
je zero
addq $16, %rsp
jmp end
zero:
popq %rax
popq %rax
movq %rax, 2(%rsp)
end:
