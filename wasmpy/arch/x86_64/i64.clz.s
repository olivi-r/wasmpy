.globl _start
_start:
pop %rax
cmp $0, %rax
je zero
bsr %rax, %rax
mov $63, %rcx
sub %rax, %rcx
push %rcx
jmp end
zero:
pushq $64
end:
