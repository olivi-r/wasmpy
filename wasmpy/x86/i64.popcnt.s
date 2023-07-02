.globl _start
_start:
pop %ax
pop %ax
shl $16, %eax
pop %ax
pop %cx
shl $16, %ecx
pop %cx
popcnt %eax, %eax
popcnt %ecx, %ecx
add %ecx, %eax
push %ax
pushw $0
pushw $0
pushw $0
pushw $4
