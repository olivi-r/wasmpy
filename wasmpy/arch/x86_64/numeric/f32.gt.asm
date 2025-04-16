[BITS 64]
fld dword [rsp]
add rsp, 4
fld dword [rsp]
fcomip
fstp dword [rsp]
ja true
mov dword [rsp], 0
jmp end
true:
mov dword [rsp], 1
end:
