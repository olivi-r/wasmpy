.globl _start
_start:
pop %ecx
pop %eax
pop %eax
pop %edx
shrd %edx, %eax
sar %cl, %edx
test $32, %cl
je switch
push %edx
sarl $31, (%esp)
push %edx
jmp end
switch:
push %edx
push %eax
end:
