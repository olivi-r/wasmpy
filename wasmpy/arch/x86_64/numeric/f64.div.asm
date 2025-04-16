[BITS 64]
fld qword [rsp]
add rsp, 8
fdivr qword [rsp]
fst qword [rsp]
