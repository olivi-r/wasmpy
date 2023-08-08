.globl _start
_start:
movb $0x80, %al
and 3(%rsp), %al
cmpb $0, %al
movl (%rsp), %eax
je zero
movl $0xffffffff, (%rsp)
jmp end
zero:
movl $0, (%rsp)
end:
subq $4, %rsp
movl %eax, (%rsp)
