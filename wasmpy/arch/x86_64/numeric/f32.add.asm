[BITS 64]
fld dword [rsp]
add rsp, 4
fadd dword [rsp]
fstp dword [rsp]
