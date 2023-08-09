.globl _start
_start:
mov (%esp), %ecx
add $4, %esp
mov (%esp), %eax
ror %cl, %eax
mov %eax, (%esp)
