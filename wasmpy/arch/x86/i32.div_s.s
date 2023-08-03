.globl _start
_start:
pop %cx
pop %cx
shl $16, %ecx
pop %cx
pop %ax
pop %ax
shl $16, %eax
pop %ax
cmp $0, %ecx
jne nezero
movl $0xff000000, %eax
movl %ebp, %esp
pop %ebp
ret
nezero:
cmp $0x80000000, %eax
jne cont
cmp $0xFFFFFFFF, %ecx
jne cont
movl $0xff0000ff, %eax
movl %ebp, %esp
pop %ebp
ret
cont:
movl $0, %edx
idiv %ecx
push %ax
shr $16, %eax
push %ax
pushw $2
