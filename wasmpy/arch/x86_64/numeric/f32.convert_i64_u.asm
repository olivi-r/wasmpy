[BITS 64]
mov qword rax, [rsp]
add rsp, 4
test rax, rax
js negative
cvtsi2ss xmm0, rax
jmp end
negative:
mov rcx, rax
and eax, 1
shr rcx, 1
or rcx, rax
cvtsi2ss xmm0, rcx
addss xmm0, xmm0
end:
movss [rsp], xmm0
