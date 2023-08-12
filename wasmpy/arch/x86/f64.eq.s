.globl _start
_start:
pop %eax
pop %ecx
pop %esi
pop %edi
cmp %eax, %esi
jne false
cmp %ecx, %edi
jne false
pushl $1
jmp end
false:
pushl $0
end:
