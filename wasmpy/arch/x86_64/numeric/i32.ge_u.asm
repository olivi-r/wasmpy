[BITS 64]
mov eax, [rsp]
add rsp, 4
cmp [rsp], eax
jae true
mov dword [rsp], 0
jmp end
true:
mov dword [rsp], 1
end:
