.globl _start
_start:
pop %eax
pop %ecx
cmp %eax, %ecx
jle true
pushl $0
jmp end
true:
pushl $1
end:
