.globl _start
_start:
pop %eax
pop %ecx
pop %edx
pop %ebx
cmp %eax, %edx
jb true
ja false
cmp %ecx, %ebx
jbe true
false:
pushl $0
jmp end
true:
pushl $1
end:
