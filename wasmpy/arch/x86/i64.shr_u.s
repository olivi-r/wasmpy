.globl _start
_start:
pop %ecx
pop %eax
pop %eax
pop %edx
shrd %edx, %eax
shr %cl, %edx
test $32, %cl
je switch
pushl $0
push %edx
jmp end
switch:
push %edx
push %eax
end:
