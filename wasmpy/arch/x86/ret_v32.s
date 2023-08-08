.globl _start
_start:
popl %eax
movl %eax, (0xff000000)
movl $0xff0000ff, %eax
leave
ret
