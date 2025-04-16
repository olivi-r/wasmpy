[BITS 64]
cmp dword [rsp], 0
je true
mov dword [rsp], 0
jmp end
true:
mov dword [rsp], 1
end:
