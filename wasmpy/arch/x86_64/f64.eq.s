.globl _start
_start:
pop %rax
pop %rcx
sub $4, %rsp
cmp %rax, %rcx
je true
movl $0, (%rsp)
jmp end
true:
movl $1, (%rsp)
end:
