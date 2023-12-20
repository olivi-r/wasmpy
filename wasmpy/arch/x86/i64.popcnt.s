.globl _start
_start:
pop %eax
pop %ecx
popcnt %eax, %eax
popcnt %ecx, %ecx
add %ecx, %eax
pushl $0
pushl %eax
