.globl _start
_start:
cmpl $0, (%rsp)
je zero
add $8, %rsp
jmp end
zero:
mov 4(%rsp), %eax
add $8, %rsp
mov %eax, (%rsp)
end:
