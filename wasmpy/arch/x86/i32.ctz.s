.globl _start
_start:
popl %eax
cmp $0, %eax
je zero
bsf %eax, %eax
push %eax
jmp end
zero:
pushl $32
end:
