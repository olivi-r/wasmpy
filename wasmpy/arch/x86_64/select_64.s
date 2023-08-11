.globl _start
_start:
cmpl $0, (%rsp)
je zero
add $12, %rsp
jmp end
zero:
add $4, %rsp
pop %rax
mov %rax, (%rsp)
end:
