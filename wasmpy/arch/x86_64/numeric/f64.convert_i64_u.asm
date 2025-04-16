[BITS 64]
mov rax, [rsp]
test rax, rax
js negative
cvtsi2sd xmm0, rax
jmp end
negative:
mov rcx, rax
and eax, 1
shr rcx, 1
or rcx, rax
cvtsi2sd xmm0, rcx
addsd xmm0, xmm0
end:
movsd [rsp], xmm0
