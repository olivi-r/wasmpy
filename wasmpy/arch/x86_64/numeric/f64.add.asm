[BITS 64]
fld qword [rsp]
add rsp, 8
fadd qword [rsp]
fstp qword [rsp]
