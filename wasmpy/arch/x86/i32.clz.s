.globl _start
_start:
popl %eax
cmp $0, %eax
je zero
bsr %eax, %eax
movl $31, %ecx
subl %eax, %ecx
pushl %ecx
jmp end
zero:
pushl $32
end:
