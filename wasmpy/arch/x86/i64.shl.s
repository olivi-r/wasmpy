.globl _start
_start:
pop %ecx
pop %eax
pop %eax
pop %edx
shld %eax, %edx
sal %cl, %eax
test $32, %cl
je switch
push %eax
pushl $0
jmp end
switch:
push %edx
push %eax
end:
