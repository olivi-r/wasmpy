[BITS 64]
mov eax, [rsp]
add rsp, 4
cmp [rsp], eax
je false
mov dword [rsp], 1
jmp end
false:
mov dword [rsp], 0
end:
