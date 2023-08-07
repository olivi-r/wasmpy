.globl _start
_start:
popl %eax
popl %ecx
cmp %ecx, %eax
je false
pushl $1
jmp end
false:
pushl $0
end:
