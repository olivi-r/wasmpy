.globl _start
_start:
pop %eax
cmp $0, %eax
je true
pushl $0
jmp end
true:
pushl $1
end:
