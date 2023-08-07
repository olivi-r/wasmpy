.globl _start
_start:
cmpl $0, (%esp)
je zero
addl $12, %esp
jmp end
zero:
popl %eax
popl %eax
popl %ecx
popl %edx
popl %edx
pushl %ecx
pushl %eax
end:
