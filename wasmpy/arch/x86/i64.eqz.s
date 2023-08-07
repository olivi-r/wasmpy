.globl _start
_start:
popl %eax
cmpl $0, %eax
popl %eax
jne false
cmpl $0, %eax
jne false
pushl $1
jmp end
false:
pushl $0
end:
