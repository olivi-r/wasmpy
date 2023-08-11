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
movl $0, (%rsp)
jmp end
zero:
mov $0xff00000000000000, %rax
leave
ret
cont:
cdq
idiv %ecx
mov %edx, (%rsp)
end:
