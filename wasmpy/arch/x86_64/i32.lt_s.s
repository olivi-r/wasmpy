.globl _start
_start:
mov (%rsp), %eax
add $4, %rsp
cmp %eax, (%rsp)
jl true
movl $0, (%rsp)
jmp end
true:
movl $1, (%rsp)
end:
