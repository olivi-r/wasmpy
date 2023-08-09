.globl _start
_start:
movl (%rsp), %eax
cmp $0, %eax
je zero
bsf %eax, %eax
movl %eax, (%rsp)
jmp end
zero:
movl $32, (%rsp)
end:
