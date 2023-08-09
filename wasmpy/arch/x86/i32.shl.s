.globl _start
_start:
mov (%esp), %ecx
add $4, %esp
mov (%esp), %eax
shl %cl, %eax
mov %eax, (%esp)
