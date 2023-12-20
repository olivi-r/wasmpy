.globl _start
_start:
pop %eax
mov %eax, (0xff000000)
mov $0xff0000ff, %eax
leave
ret
