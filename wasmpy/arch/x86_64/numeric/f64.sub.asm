[BITS 64]
fld qword [rsp]
add rsp, 8
fsubr qword [rsp]
fstp qword [rsp]
