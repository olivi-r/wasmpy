.globl _start
_start:
mov (%rsp), %eax
cmp $0, %eax
je zero
bsf %eax, %eax
mov %eax, (%rsp)
jmp end
zero:
movl $32, (%rsp)
end:
