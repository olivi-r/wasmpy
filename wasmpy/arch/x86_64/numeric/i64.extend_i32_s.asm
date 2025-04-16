[BITS 64]
mov al, 0x80
and al, 3[rsp]
cmp al, 0
mov eax, [rsp]
je zero
mov dword [rsp], 0xffffffff
jmp end
zero:
mov dword [rsp], 0
end:
sub rsp, 4
mov [rsp], eax
