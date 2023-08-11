.globl _start
_start:
mov $0x80, %al
and 3(%rsp), %al
cmp $0, %al
mov (%rsp), %eax
je zero
movl $0xffffffff, (%rsp)
jmp end
zero:
movl $0, (%rsp)
end:
sub $4, %rsp
mov %eax, (%rsp)
