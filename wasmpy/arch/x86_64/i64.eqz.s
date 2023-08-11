.globl _start
_start:
pop %rax
sub $4, %rsp
cmp $0, %rax
je true
movl $0, (%rsp)
jmp end
true:
movl $1, (%rsp)
end:
