[BITS 64]
mov eax, [rsp]
cmp eax, 0
je zero
bsf eax, eax
mov [rsp], eax
jmp end
zero:
mov dword [rsp], 32
end:
