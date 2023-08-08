.globl _start
_start:
movl (%rsp), %eax
cmp $0, %eax
je zero
bsr %eax, %eax
movl $31, %ecx
subl %eax, %ecx
movl %ecx, (%rsp)
jmp end
zero:
movl $32, (%rsp)
end:
