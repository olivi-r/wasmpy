[BITS 64]
fld qword [rsp]
fld qword 8[rsp]
add rsp, 12
fcomip
fstp dword [rsp]
jae true
mov dword [rsp], 0
jmp end
true:
mov dword [rsp], 1
end:
