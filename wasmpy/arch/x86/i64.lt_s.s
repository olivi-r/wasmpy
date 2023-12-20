.globl _start
_start:
pop %eax
pop %ecx
pop %edx
pop %ebx
cmp %eax, %edx
jl true
jg false
cmp %ecx, %ebx
jb true
false:
pushl $0
jmp end
true:
pushl $1
end:
