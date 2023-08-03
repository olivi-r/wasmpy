.globl _start
_start:
pop %ax
pop %ax
shl $16, %eax
pop %ax
popcnt %eax, %eax
push %ax
pushw $0
pushw $2
