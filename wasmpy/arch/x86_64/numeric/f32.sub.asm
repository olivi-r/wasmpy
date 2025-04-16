[BITS 64]
fld dword [rsp]
add rsp, 4
fsubr dword [rsp]
fstp dword [rsp]
