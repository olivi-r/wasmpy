[BITS 32]
fld qword [esp]
pop eax
fabs
fisttp dword [esp]
