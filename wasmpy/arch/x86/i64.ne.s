.globl _start
_start:
pop %eax
pop %ecx
pop %esi
pop %edi
cmp %eax, %esi
jne true
cmp %ecx, %edi
jne true
pushl $0
jmp end
true:
pushl $1
end:
