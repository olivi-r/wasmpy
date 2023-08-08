.globl _start
_start:
popq %rax
popcnt %rax, %rax
pushq %rax
