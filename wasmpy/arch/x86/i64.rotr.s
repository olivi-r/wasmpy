.globl _start
_start:
pop %ecx
pop %eax
pop %eax
pop %edx
xor %ebx, %ebx
shrd %eax, %ebx
shrd %edx, %eax
shr %cl, %edx
or %ebx, %edx
test $32, %cl
jne switch
push %edx
switch:
push %eax
je end
push %edx
end:
