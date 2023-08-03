.globl _start
_start:
pop %dx
pop %dx
shl $16, %edx
pop %dx
pop %ax
shl $16, %eax
pop %ax
movl %eax, (0xff000000)
movl %edx, %eax
movl %eax, (0xff0000ff)
movl $0xff00ff00, %eax
movl %ebp, %esp
pop %ebp
ret
