.globl _start
_start:
pop %eax
pop %ecx
pop %edx
pop %ebx
cmp %eax, %edx
ja true
jb false
cmp %ecx, %ebx
jae true
false:
pushl $0
jmp end
true:
pushl $1
end:
