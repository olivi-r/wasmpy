.globl _start
_start:
pop %rax
cmp $0, %rax
je zero
bsf %rax, %rax
push %rax
jmp end
zero:
pushq $64
end:
