.globl _start
_start:
pop %ecx
pop %eax
pop %eax
shld %eax, (%esp)
shl %cl, %eax
test $32, %cl
jne switch
push %eax
jmp end
switch:
pop %ebx
push %eax
push %edx
end:
