.globl _start
_start:
movl $0xff000000, %eax
movl %ebp, %esp
pop %ebp
ret