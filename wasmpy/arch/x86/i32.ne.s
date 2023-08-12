.globl _start
_start:
pop %eax
pop %ecx
cmp %ecx, %eax
je false
pushl $1
jmp end
false:
pushl $0
end:
