.globl _start
_start:
pop %eax
pop %ecx
cmp %eax, %ecx
jl true
pushl $0
jmp end
true:
pushl $1
end:
