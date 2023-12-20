.globl _start
_start:
mov (%rsp), %eax
add $4, %rsp
cmp %eax, (%rsp)
je false
movl $1, (%rsp)
jmp end
false:
movl $0, (%rsp)
end:
