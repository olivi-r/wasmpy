.globl _start
_start:
pop %eax
mov %eax, (0xff000000)
pop %eax
mov %eax, (0xff0000ff)
mov $0xff00ff00, %eax
leave
ret
