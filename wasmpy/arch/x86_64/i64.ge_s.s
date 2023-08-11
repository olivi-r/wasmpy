.globl _start
_start:
pop %rax
pop %rcx
cmp %rax, %rcx
sub $4, %rsp
jge true
movl $0, (%rsp)
jmp end
true:
movl $1, (%rsp)
end:
