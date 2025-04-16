[BITS 64]
fld qword [rsp]
add rsp, 8
fmul qword [rsp]
fst qword [rsp]
