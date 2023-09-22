.globl _start
_start:
mov $0xFF000000, %eax
mov (%eax), %eax
push %eax
