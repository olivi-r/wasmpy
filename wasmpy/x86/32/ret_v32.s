.globl _start
_start:
pop %ax
pop %ax
shl $16, %eax
pop %ax
movl %eax, (0xff000000)
movl $0xff0000ff, %eax
movl %ebp, %esp
pop %ebp
ret
