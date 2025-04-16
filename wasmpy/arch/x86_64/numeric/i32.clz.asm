[BITS 64]
mov eax, [rsp]
cmp eax, 0
je zero
bsr eax, eax
mov ecx, 31
sub ecx, eax
mov [rsp], ecx
jmp end
zero:
mov dword [rsp], 32
end:
