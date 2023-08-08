.globl _start
_start:
movl (%rsp), %eax
popcnt %eax, %eax
movl %eax, (%rsp)
