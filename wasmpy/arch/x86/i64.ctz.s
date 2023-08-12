.globl _start
_start:
pop %ecx
pop %eax
cmp $0, %eax
je highzero
bsf %eax, %eax
add $32, %eax
pushl $0
push %eax
jmp end
highzero:
cmp $0, %ecx
je lowzero
bsf %ecx, %ecx
pushl $0
push %ecx
jmp end
lowzero:
pushl $0
pushl $64
end:
