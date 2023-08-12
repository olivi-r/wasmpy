.globl _start
_start:
pop %eax
pop %ecx
cmp $0, %ecx
je highzero
bsr %ecx, %ecx
mov $31, %eax
sub %ecx, %eax
pushl $0
push %eax
jmp end
highzero:
cmp $0, %eax
je zero
bsr %eax, %eax
mov $63, %ecx
sub %eax, %ecx
pushl $0
push %ecx
jmp end
zero:
pushl $0
pushl $64
end:
