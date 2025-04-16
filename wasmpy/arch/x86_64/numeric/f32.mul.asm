[BITS 64]
fld dword [rsp]
add rsp, 4
fmul dword [rsp]
fst dword [rsp]
