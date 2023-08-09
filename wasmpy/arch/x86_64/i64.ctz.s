.globl _start
_start:
popq %rax
cmp $0, %rax
je zero
bsf %rax, %rax
pushq %rax
jmp end
zero:
pushq $64
end:
