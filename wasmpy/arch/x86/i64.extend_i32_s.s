.globl _start
_start:
pop %ax
pop %ax
push %ax
and $0x8000, %ax
cmp $0, %ax
je zero
pushw $0xffff
pushw $0xffff
jmp end
zero:
pushw $0
pushw $0
end:
pushw $4
