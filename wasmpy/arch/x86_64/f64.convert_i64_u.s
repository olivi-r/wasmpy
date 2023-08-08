.globl _start
_start:
movq (%rsp), %rax
testq %rax, %rax
js negative
cvtsi2sdq %rax, %xmm0
jmp end
negative:
movq %rax, %rcx
andl $1, %eax
shrq %rcx
orq %rax, %rcx
cvtsi2sdq %rcx, %xmm0
addsd %xmm0, %xmm0
end:
movsd %xmm0, (%rsp)
