.globl _start
_start:
cmpl $0, (%rsp)
je true
movl $0, (%rsp)
jmp end
true:
movl $1, (%rsp)
end:
