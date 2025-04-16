[BITS 64]
fild dword [rsp]
sub rsp, 4
fstp qword [rsp]
