.globl _start
_start:
popl %ecx
popl %eax
cmpl $0, %eax
je highzero
bsf %eax, %eax
add $32, %eax
pushl $0
pushl %eax
jmp end
highzero:
cmpl $0, %ecx
je lowzero
bsf %ecx, %ecx
pushl $0
pushl %ecx
jmp end
lowzero:
pushl $0
pushl $64
end:
