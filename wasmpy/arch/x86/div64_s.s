.globl _start
_start:
cmpl $0, (%esp)
jne continue1
cmpl $0, 4(%esp)
jne continue1
mov $0xFF000000, %eax
leave
ret
continue1:
cmpl $0xFFFFFFFF, (%esp)
jne continue2
cmpl $0xFFFFFFFF, 4(%esp)
jne continue2
cmpl $0, 8(%esp)
jne continue2
cmpl $0x80000000, 12(%esp)
jne continue2
mov $0xFF0000FF, %eax
leave
ret
continue2:
mov $0xFF00FF00, %ebx
call *%ebx
pop %ecx
pop %ecx
push %edx
push %eax
