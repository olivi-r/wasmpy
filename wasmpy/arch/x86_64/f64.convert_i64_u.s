.globl _start
_start:
mov (%rsp), %rax
test %rax, %rax
js negative
cvtsi2sd %rax, %xmm0
jmp end
negative:
mov %rax, %rcx
and $1, %eax
shr %rcx
or %rax, %rcx
cvtsi2sd %rcx, %xmm0
addsd %xmm0, %xmm0
end:
movsd %xmm0, (%rsp)
