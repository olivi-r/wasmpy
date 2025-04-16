[BITS 64]
fild qword [rsp]
add rsp, 4
fstp dword [rsp]
