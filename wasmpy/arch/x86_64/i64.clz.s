.globl _start
_start:
popq %rax
cmp $0, %rax
je zero
bsr %rax, %rax
movq $63, %rcx
subq %rax, %rcx
pushq %rcx
jmp end
zero:
pushq $64
end:
