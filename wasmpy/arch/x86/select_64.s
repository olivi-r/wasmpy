.globl _start
_start:
pop %eax
cmp $0, %eax
pop %eax
pop %ecx
je zero
jmp end
zero:
pop %edx
pop %edx
push %ecx
push %eax
end:
