.globl _start
_start:
movq (%rsp), %rax
add $4, %rsp
test %rax, %rax
js negative
cvtsi2ss %rax, %xmm0
jmp end
negative:
mov %rax, %rcx
and $1, %eax
shr %rcx
or %rax, %rcx
cvtsi2ss %rcx, %xmm0
addss %xmm0, %xmm0
end:
movss %xmm0, (%rsp)
