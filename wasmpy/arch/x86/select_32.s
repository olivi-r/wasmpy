.globl _start
_start:
cmpl $0, (%esp)
je zero
addl $8, %esp
jmp end
zero:
popl %eax
popl %eax
popl %ecx
pushl %eax
end:
