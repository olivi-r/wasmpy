.globl _start
_start:
mov (%rsp), %eax
popcnt %eax, %eax
mov %eax, (%rsp)
