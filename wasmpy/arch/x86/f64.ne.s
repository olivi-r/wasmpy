.globl _start
_start:
popl %eax
popl %ecx
popl %esi
popl %edi
cmp %eax, %esi
jne true
cmp %ecx, %edi
jne true
pushl $0
jmp end
true:
pushl $1
end:
