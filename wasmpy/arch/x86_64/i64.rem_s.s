.globl _start
_start:
pop %rcx
pop %rax
cmp $0, %rcx
je zero
mov $0x8000000000000000, %rdx
cmp %rdx, %rax
jne cont
cmp $0xffffffffffffffff, %rcx
jne cont
pushq $0
jmp end
zero:
mov $0xff00000000000000, %rax
leave
ret
cont:
cqo
idiv %rcx
push %rdx
end:
