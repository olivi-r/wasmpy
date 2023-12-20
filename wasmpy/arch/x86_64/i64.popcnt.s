.globl _start
_start:
pop %rax
popcnt %rax, %rax
push %rax
