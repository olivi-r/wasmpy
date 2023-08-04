.globl _start
_start:
cmpl $0, 2(%esp)
je zero
addl $12, %esp
jmp end
zero:
movl 8(%esp), %eax
movl %eax, 14(%esp)
addl $12, %esp
end:
