.globl _start
_start:
popl %eax
popl %ecx
popl %esi
popl %edi
cmpl %eax, %esi
jne false
cmpl %ecx, %edi
jne false
pushl $1
jmp end
false:
pushl $0
end:
