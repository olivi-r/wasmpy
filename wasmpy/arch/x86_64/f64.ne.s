.globl _start
_start:
pop %rax
pop %rcx
sub $4, %rsp
cmp %rcx, %rax
je false
movl $1, (%rsp)
jmp end
false:
movl $0, (%rsp)
end:
