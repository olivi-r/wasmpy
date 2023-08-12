.globl _start
_start:
pop %eax
popcnt %eax, %eax
push %eax
