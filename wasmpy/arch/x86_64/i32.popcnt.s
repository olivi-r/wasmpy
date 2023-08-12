.globl _start
_start:
popcnt (%rsp), %eax
mov %eax, (%rsp)
