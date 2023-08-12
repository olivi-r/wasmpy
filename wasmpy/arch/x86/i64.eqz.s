.globl _start
_start:
pop %eax
cmp $0, %eax
pop %eax
jne false
cmp $0, %eax
jne false
pushl $1
jmp end
false:
pushl $0
end:
