.globl _start
_start:
pop %eax
cmp $0, %eax
pop %eax
jne end
pop %ecx
push %eax
end:
