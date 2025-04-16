[BITS 64]
fld dword [rsp]
add rsp, 4
fdivr dword [rsp]
fst dword [rsp]
