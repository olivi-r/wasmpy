.globl _start
_start:
mov (%esp), %ecx
add $4, %esp
mov (%esp), %eax
shr %cl, %eax
mov %eax, (%esp)
