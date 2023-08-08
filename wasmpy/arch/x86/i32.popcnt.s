.globl _start
_start:
popl %eax
popcnt %eax, %eax
pushl %eax
