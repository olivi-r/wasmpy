[BITS 32]
fld dword [esp]
push eax
fabs
fisttp qword [esp]
