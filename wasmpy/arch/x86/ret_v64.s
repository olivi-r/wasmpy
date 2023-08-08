.globl _start
_start:
popl %eax
movl %eax, (0xff000000)
popl %eax
movl %eax, (0xff0000ff)
movl $0xff00ff00, %eax
leave
ret
