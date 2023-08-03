.globl _start
_start:
pop %ax
pop %ax
shl $16, %eax
pop %ax
pop %cx
shl $16, %ecx
pop %cx
cmp $0, %eax
je highzero
bsf %eax, %eax
add $32, %ax
push %ax
jmp end
highzero:
cmp $0, %ecx
je lowzero
bsf %ecx, %ecx
push %cx
jmp end
lowzero:
pushw $64
end:
pushw $0
pushw $0
pushw $0
pushw $4
