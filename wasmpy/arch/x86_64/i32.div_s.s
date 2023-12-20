.globl _start
_start:
mov (%rsp), %ecx
add $4, %rsp
mov (%rsp), %eax
cmp $0, %ecx
je zero
cmp $0x80000000, %eax
jne cont
cmp $0xffffffff, %ecx
jne cont
mov $0xff000000000000ff, %rax
leave
ret
zero:
mov $0xff00000000000000, %rax
leave
ret
cont:
cdq
idiv %ecx
mov %eax, (%rsp)
