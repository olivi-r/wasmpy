.globl _start
_start:
pop %eax
cmp $0, %eax
je zero
bsr %eax, %eax
mov $31, %ecx
sub %eax, %ecx
push %ecx
jmp end
zero:
pushl $32
end:
