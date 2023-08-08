.globl _start
_start:
movq (%rsp), %rax
addq $4, %rsp
testq %rax, %rax
js negative
cvtsi2ssq %rax, %xmm0
jmp end
negative:
movq %rax, %rcx
andl $1, %eax
shrq %rcx
orq %rax, %rcx
cvtsi2ssq %rcx, %xmm0
addss %xmm0, %xmm0
end:
movss %xmm0, (%rsp)
