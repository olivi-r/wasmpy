.globl _start
_start:
pop %ecx
pop %eax
cmp $0, %ecx
jne continue
movl $0xff000000, %eax
leave
ret
continue:
xor %edx, %edx
div %ecx
push %eax
