.globl _start
_start:
pop %ecx
pop %eax
pop %eax
pop %edx
xor %ebx, %ebx
shld %edx, %ebx
shld %eax, %edx
shl %cl, %eax
or %ebx, %eax
test $32, %cl
jne switch
push %edx
switch:
push %eax
je end
push %edx
end:
