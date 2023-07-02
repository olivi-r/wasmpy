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
je end
movl $0, %edx
div %ecx
push %ax
shr $16, %eax
push %ax
pushw $2
end:
