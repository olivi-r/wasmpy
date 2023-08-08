.globl _start
_start:
popl %eax
popl %ecx
cmp $0, %ecx
je highzero
bsr %ecx, %ecx
movl $31, %eax
subl %ecx, %eax
pushl $0
pushl %eax
jmp end
highzero:
cmp $0, %eax
je zero
bsr %eax, %eax
movl $63, %ecx
subl %eax, %ecx
pushl $0
pushl %ecx
jmp end
zero:
pushl $0
pushl $64
end:
