.globl _start
_start:
mov (%rsp), %eax
cmp $0, %eax
je zero
bsr %eax, %eax
mov $31, %ecx
sub %eax, %ecx
mov %ecx, (%rsp)
jmp end
zero:
movl $32, (%rsp)
end:
