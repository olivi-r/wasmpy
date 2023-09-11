.globl _start
_start:
cmpl $0, (%esp)
jne continue
cmpl $0, 4(%esp)
jne continue
mov $0xFF000000, %eax
leave
ret
continue:
movl $0xFF0000FF, %ebx
call *%ebx
pop %ecx
pop %ecx
push %edx
push %eax
