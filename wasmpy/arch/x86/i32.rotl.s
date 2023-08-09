.globl _start
_start:
mov (%esp), %ecx
add $4, %esp
mov (%esp), %eax
rol %cl, %eax
mov %eax, (%esp)
