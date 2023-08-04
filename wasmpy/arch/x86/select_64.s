.globl _start
_start:
cmpl $0, 2(%esp)
je zero
addl $16, %esp
jmp end
zero:
addl $8, %esp
popl %eax
popl %ecx
movl %eax, 2(%esp)
movl %ecx, 6(%esp)
end:
