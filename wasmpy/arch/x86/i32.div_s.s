.globl _start
_start:
pop %ecx
pop %eax
cmp $0, %ecx
je zero
cmp $0x80000000, %eax
jne cont
cmp $0xFFFFFFFF, %ecx
jne cont
movl $0xff0000ff, %eax
leave
ret
zero:
movl $0xff000000, %eax
leave
ret
cont:
cdq
idiv %ecx
push %eax
